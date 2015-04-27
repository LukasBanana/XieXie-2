";\n"
"; Core.xasm\n"
"; \n"
"; This file is part of the \"XieXie 2.0 Project\" (Copyright (c) 2014 by Lukas Hermanns)\n"
"; See \"LICENSE.txt\" for license information.\n"
";\n"
"\n"
"; Export all global addresses\n"
";.pragma(\"export all\")\n"
"\n"
"; ------- Common -------\n"
"\n"
"; Object new(int size, int typeID, int vtableAddr)\n"
"new:\n"
"	ldw $ar, ($lb) -4	; load argument 'size' (must include 12 bytes for reference counter, type-id, and vtable)\n"
"	push $ar\n"
"	insc AllocMem		; allocate memory for object and get object reference (ar)\n"
"	\n"
"	mov $r1, 1			; initialize reference counter to 1\n"
"	stw $r1, ($ar) 0	; write reference counter to object\n"
"	\n"
"	ldw $r1, ($lb) -8	; load argument 'typeID'\n"
"	stw $r1, ($ar) 4	; write type-id to object\n"
"	\n"
"	ldw $r1, ($lb) -12	; load argument 'vtableAddr' (address to vtable for the object)\n"
"	stw $r1, ($ar) 8	; write vtable address to object\n"
"	\n"
"	ret 3				; return object reference (ar)\n"
"\n"
"; void inc_ref(Object this)\n"
"inc_ref:\n"
"	; check for null pointer\n"
"	mov $cf, $xr			; compare: this = null\n"
"	je .end\n"
"		; increment reference\n"
"		ldw $r0, ($xr) 0	; load reference counter from object\n"
"		inc $r0				; increment reference counter\n"
"		stw $r0, ($xr) 0	; write reference counter back to object\n"
"	.end:\n"
"	ret\n"
"\n"
"; void dec_ref(Object this)\n"
"dec_ref:\n"
"	; check for null pointer\n"
"	mov $cf, $xr			; compare: this = null\n"
"	je .end\n"
"	\n"
"	; check: ref-count = 1\n"
"	ldw $r0, ($xr) 0		; load reference counter from object\n"
"	sub $cf, $r0, 1			; check if reference is 1\n"
"	je .rel					; release object\n"
"		dec $r0				; decrement reference counter\n"
"		stw $r0, ($xr) 0	; write reference counter back to object\n"
"		ret\n"
"	.rel:\n"
"	\n"
"	; load d'tor address\n"
"	ldw $r0, ($xr) 8		; load vtable address\n"
"	ldw $r0, ($r0) 0		; load release procedure address\n"
"	\n"
"	; call d'tor\n"
"	push $xr				; store $xr\n"
"	call $r0				; call d'tor 'release(this)'\n"
"	;pop $xr				; restore $xr\n"
"	\n"
"	; free memory\n"
"	;push $xr				; push this pointer again as argument onto stack\n"
"	insc FreeMem			; delete object 'FreeMem(this)'\n"
"	\n"
"	.end:\n"
"	ret\n"
"\n"
"; Object dynamic_cast(Object obj, int baseTypeID, int castNumSubClasses)\n"
"dynamic_cast:\n"
"	ldw $cf, ($lb) -4		; load object reference 'obj'\n"
"	\n"
"	; check for null pointer\n"
"	je .end					; compare: obj = null\n"
"	\n"
"	; load type ids\n"
"	mov $ar, $cf\n"
"	ldw $r1, ($ar) 4		; load type-id of 'obj'\n"
"	ldw $r2, ($lb) -8		; load type-id 'baseTypeID'\n"
"	ldw $r3, ($lb) -12		; load number of sub classes 'castNumSubClasses'\n"
"	add $r3, $r3, $r2		; calculate: baseTypeID + castNumSubClasses\n"
"	\n"
"	; compare type-ids\n"
"	cmp $r2, $r1			; compare: baseTypeID > obj.typeID\n"
"	jg .end\n"
"		cmp $r1, $r3		; compare: obj.typeID > (baseTypeID + castNumSubClasses)\n"
"		jg .end\n"
"			ret 3			; return object reference (ar)\n"
"			\n"
"	; class-cast exception\n"
"	.end:\n"
"	mov $ar, 0				; return null pointer\n"
"	ret 3\n"
"\n"
"; void resize_buffer(Object this, int size)\n"
"resize_buffer:\n"
"	ldw $r0, ($lb) -4		; load size\n"
"	ldw $r1, ($xr) 16		; load this.bufSize\n"
"	\n"
"	; check if buffer must be increased (size > this.bufSize)\n"
"	cmp $r0, $r1\n"
"	jle .else\n"
"		; get further member variables\n"
"		ldw $r2, ($xr) 12	; load this.size\n"
"		ldw $r3, ($xr) 20	; load this.buffer\n"
"		\n"
"		; increase buffer (2x size pre-allocated storage)\n"
"		sll $r4, $r0, 1		; set newSize := size*2\n"
"		push $r4\n"
"		insc AllocMem		; allocate memory for 'newSize' bytes\n"
"		\n"
"		; store new values in object\n"
"		stw $ar, ($xr) 20	; store: this.buffer := newBuffer\n"
"		stw $r4, ($xr) 16	; store: this.bufSize := newSize\n"
"		\n"
"		; copy previous buffer into new one\n"
"		push $r2			; push previous buffer size\n"
"		push $r3			; push original buffer as source address\n"
"		push $ar			; push new buffer as destination address\n"
"		insc CopyMem\n"
"		\n"
"		; free previous buffer\n"
"		push $r3\n"
"		insc FreeMem\n"
"		\n"
"		; fill new elements with zeros\n"
"		push 0				; fill with zeros\n"
"		sub $r3, $r0, $r2	; fillSize := size - prevSize\n"
"		push $r3\n"
"		add $r3, $ar, $r2	; get address: newBuffer + prevSize\n"
"		push $r3\n"
"		insc FillMem\n"
"	jmp .end\n"
"	.else:\n"
"	\n"
"	; check if buffer should be decreased (this.bufSize > 16 AND size < this.bufSize/2)\n"
"	cmp $r1, 16\n"
"	jle .end\n"
"	slr $r1, $r1, 1\n"
"	cmp $r0, $r1\n"
"	jge .end\n"
"		; get further member variables\n"
"		ldw $r2, ($xr) 12	; load this.size\n"
"		ldw $r3, ($xr) 20	; load this.buffer\n"
"		\n"
"		; decrease buffer\n"
"		push $r0\n"
"		insc AllocMem		; allocate memory for 'size' bytes\n"
"		\n"
"		; store new values in object\n"
"		stw $ar, ($xr) 20	; store: this.buffer := newBuffer\n"
"		stw $r4, ($xr) 16	; store: this.bufSize := newSize\n"
"		\n"
"		; copy previous buffer into new one\n"
"		push $r0			; push new buffer size\n"
"		push $r3			; push original buffer as source address\n"
"		push $ar			; push new buffer as destination address\n"
"		insc CopyMem\n"
"		\n"
"		; free previous buffer\n"
"		push $r3\n"
"		insc FreeMem\n"
"	.end:\n"
"	\n"
"	stw $r0, ($xr) 12		; store: this.size := size\n"
"	\n"
"	ret 1\n"
"\n"
"; String String.copy_literal(String literal)\n"
"String.copy_literal:\n"
"	; allocate new \"String\" object\n"
"	ldw $r0, ($lb) -4					; load address of string literal\n"
"	push 24\n"
"	insc AllocMem						; allocate memory for new string instance\n"
"	mov $xr, $ar						; store pointer to new string object\n"
"	push $r0\n"
"	call CString.Pinit,R@String?source	; call copy constructor\n"
"	mov $ar, $xr						; restore and return pointer to new string object\n"
"	ret 1\n"
"\n"
"\n"
"; ------- Object Class -------\n"
"\n"
"; Object:\n"
";	int refCount	(0)\n"
";	int typeID		(4)\n"
";	int vtableAddr	(8)\n"
"\n"
"; void Object.release(Object this)\n"
"CObject.Prelease:\n"
"\n"
"; void Object.init(Object this)\n"
"CObject.Pinit:\n"
"	ret\n"
"\n"
"; int Object.refCount(Object this)\n"
"CObject.PrefCount:\n"
"	ldw $ar, ($xr) 0	; load and return reference counter of 'this'\n"
"	ret\n"
"\n"
"; int Object.typeID(Object this)\n"
"CObject.PtypeID:\n"
"	ldw $ar, ($xr) 4	; load and return type-id of 'this'\n"
"	ret\n"
"\n"
"; bool Object.equals(Object this, Object rhs)\n"
"CObject.Pequals,R@Object?rhs:\n"
"	; check for null pointer\n"
"	ldw $cf, ($lb) -4	; load object reference 'rhs' and compare: rhs = null\n"
"	je .false\n"
"	\n"
"	; load type ids\n"
"	ldw $r0, ($xr) 4	; load type-id of 'this'\n"
"	ldw $r1, ($cf) 4	; load type-id of 'rhs'\n"
"	\n"
"	; check: this.typeID = rhs.typeID\n"
"	cmp $r0, $r1		; compare type-id of both object references\n"
"	jne .false\n"
"		mov $ar, 1		; return true\n"
"		ret 1\n"
"	.false:\n"
"	mov $ar, 0			; return false\n"
"	ret 1\n"
"\n"
"; TODO: currently returns null pointer !!!\n"
"; String Object.toString(Object this)\n"
"CObject.PtoString:\n"
"	mov $ar, 0	; return null pointer\n"
"	ret\n"
"\n"
"; int Object.pointer(Object this)\n"
"CObject.Ppointer:\n"
"	add $ar, $xr, 12	; return pointer to first member variables of derived class\n"
"	ret\n"
"\n"
"Object.vtable:\n"
"	.word @CObject.Prelease\n"
"	.word @CObject.Pinit\n"
"	.word @CObject.PtypeID\n"
"	.word @CObject.PrefCount\n"
"	.word @CObject.Pequals,R@Object?rhs\n"
"	.word @CObject.PtoString\n"
"	.word @CObject.Ppointer\n"
"\n"
"\n"
"; ------- String Class -------\n"
"\n"
"; String:\n"
";	Object	super	( 0)\n"
";	int		size	(12) <- buffer size (in bytes)\n"
";	int		bufSize	(16) <- actual buffer size (in bytes)\n"
";	int		buffer	(20)\n"
"\n"
"; void String.release(Object this)\n"
"CString.Prelease:\n"
"	; free string buffer\n"
"	ldw $r0, ($xr) 20	; load pointer to string buffer\n"
"	push $r0\n"
"	insc FreeMem		; delete string buffer\n"
"	ret\n"
"\n"
"; void String.init(Object this)\n"
"CString.Pinit:\n"
"	; this.size := 1 (null character)\n"
"	mov $r0, 1\n"
"	stw $r0, ($xr) 12\n"
"	\n"
"	; initialize string buffer of 16 bytes\n"
"	mov $r1, 16\n"
"	stw $r1, ($xr) 16\n"
"	\n"
"	; allocate string buffer\n"
"	push 16\n"
"	insc AllocMem		; allocate 16 bytes\n"
"	stw $ar, ($xr) 20	; store pointer to buffer in string instance\n"
"	\n"
"	; initialize string buffer with null terminator\n"
"	stw $r0, ($ar) 0	; this.pointer[0] := 0\n"
"	ret\n"
"\n"
"; bool String.equals(String this, Object rhs)\n"
"CString.Pequals,R@Object?rhs:\n"
"	; check for null pointer\n"
"	ldw $cf, ($lb) -4		; load object reference 'rhs' and compare: rhs = null\n"
"	je .false\n"
"	\n"
"	; compare type-id of both object references\n"
"	mov $tr, $cf\n"
"	ldw $r0, ($xr) 4		; load type-id of 'this'\n"
"	ldw $r1, ($tr) 4		; load type-id of 'rhs'\n"
"	cmp $r0, $r1			; check: this.typeID != rhs.typeID	\n"
"	jne .false\n"
"	\n"
"	; compare size of both object references\n"
"	ldw $r0, ($xr) 12		; load this.size\n"
"	ldw $r1, ($tr) 12		; load rhs.size\n"
"	cmp $r0, $r1			; check: this.size != rhs.size\n"
"	jne .false\n"
"	\n"
"	; compare characters of both string\n"
"	ldw $r2, ($xr) 20		; set ptrA := this.buffer\n"
"	ldw $r3, ($tr) 20		; set ptrB := rhs.buffer\n"
"	.loop:\n"
"		; loop break condition\n"
"		mov $cf, $r0\n"
"		jle .break			; check if size <= 0\n"
"		\n"
"		; compare current two characters on equality\n"
"		ldw $r4, ($r2) 0	; chrA := load ptrA[i]\n"
"		ldw $r5, ($r3) 0	; chrB := load ptrB[i]\n"
"		cmp $r4, $r5		; check chrA != chrB\n"
"		jne .false\n"
"		\n"
"		; next iteration\n"
"		inc $r2				; ptrA++\n"
"		inc $r3				; ptrB++\n"
"		dec $r0				; size--\n"
"	jmp .loop\n"
"	.break:\n"
"	\n"
"	; return true\n"
"	mov $ar, 1\n"
"	ret 1\n"
"	\n"
"	; return false\n"
"	.false:\n"
"	mov $ar, 0\n"
"	ret 1\n"
"\n"
"; void String.init(String this, int size)\n"
"CString.Pinit,I?size:\n"
"	call CString.Pinit\n"
"	call CString.Presize,I?size\n"
"	ret\n"
"\n"
"; void String.init(String this, String source)\n"
"CString.Pinit,R@String?source:\n"
"	; copy first 20 bytes from literal\n"
"	ldw $r0, ($lb) -4	; load address of string literal\n"
"	push 20				; push size of 20 bytes (excluding the '.buffer' address)\n"
"	push $r0			; push source address of original string literal\n"
"	push $xr			; push this pointer as destination address\n"
"	insc CopyMem\n"
"	\n"
"	; allocate buffer for new string\n"
"	ldw $r1, ($r0) 12	; load 'String.size'\n"
"	inc $r1				; increase size for null terminator\n"
"	push $r1\n"
"	insc AllocMem		; allocate buffer\n"
"	stw $ar, ($xr) 20	; write buffer address to new string object\n"
"	\n"
"	; copy string buffer from literal\n"
"	ldw $r0, ($r0) 20	; load buffer address from literal\n"
"	push $r1			; push buffer size\n"
"	push $r0			; push buffer address of literal\n"
"	push $ar			; push buffer address of new string\n"
"	insc CopyMem\n"
"	ret 1\n"
"\n"
"; String String.toString(Object this)\n"
"CString.PtoString:\n"
"	call inc_ref	; increment reference of this (return strong this)\n"
"	mov $ar, $xr	; just return this pointer\n"
"	ret\n"
"\n"
"; String String.copy(String this)\n"
"CString.Pcopy:\n"
"	; allocate new String instance\n"
"	push @String.vtable					; push String vtable\n"
"	push 1								; push String type-id\n"
"	push 24								; push String size\n"
"	call new							; allocate new String\n"
"	pop $r1\n"
"	\n"
"	; call String's copy c'tor\n"
"	ldw $r0, ($lb) -4					; load this pointer\n"
"	push $r1							; push first as back-up\n"
"	push $r1							; push second as argument\n"
"	mov $xr, $r0						; override this pointer with reference to new String instance\n"
"	call CString.Pinit,R@String?source	; call String copy constructor\n"
"	\n"
"	; return new String instance\n"
"	pop $ar								; get return value from back-up $r0 on stack\n"
"	ret\n"
"\n"
"; int String.size(String this)\n"
"CString.Psize:\n"
"	ldw $ar, ($xr) 12	; load and return member this.size\n"
"	dec $ar				; remove null terminator from character counter\n"
"	ret\n"
"\n"
"; void String.resize(String this, int size)\n"
"CString.Presize,I?size:\n"
"	ldw $cf, ($lb) -4		; load size\n"
"	jge .else				; check: size >= 0\n"
"		mov $cf, 0			; clamp size to 0\n"
"	.else:\n"
"	add $tr, $cf, 1			; set tr := size + 1 (null terminator)\n"
"	ldw $cf, ($xr) 12		; load this.size\n"
"	cmp $cf, $tr			; compare this.size = size\n"
"	je .end\n"
"		push $tr\n"
"		call resize_buffer	; resize string buffer\n"
"		ldw $r0, ($xr) 20	; load this.buffer\n"
"		add $r0, $r0, $tr	; add offset to buffer\n"
"		mov $tr, 0\n"
"		stw $tr, ($r0) -1	; store null terminator at this.buffer[tr - 1]\n"
"	.end:\n"
"	ret 1\n"
"\n"
"; void String.adjust()\n"
"CString.Padjust:\n"
"	; ...\n"
"	ret\n"
"\n"
"; bool String.empty(String this)\n"
"CString.Pempty:\n"
"	ldw $cf, ($xr) 12	; load member this.size\n"
"	jg .not_empty		; compare this.size > 0\n"
"		mov $ar, 1		; return true\n"
"		ret\n"
"	.not_empty:\n"
"	mov $ar, 0			; return false\n"
"	ret\n"
"\n"
"; String String.append(String this, String rhs)\n"
"CString.Pappend,R@String?rhs:\n"
"	ldw $cf, ($lb) -4	; load rhs\n"
"	je .end				; check if rhs = null\n"
"	mov $r0, $cf		; store rhs\n"
"	\n"
"	; check if other string is empty\n"
"	ldw $r1, ($r0) 12	; load rhs.size\n"
"	cmp $r1, 1			; check: rhs.size <= 1\n"
"	jle .end\n"
"	\n"
"	; determine new string size\n"
"	ldw $r2, ($xr) 12	; load this.size\n"
"	add $r1, $r1, $r2	; calculate size := this.size + rhs.size\n"
"	sub $r1, $r1, 2		; subtract 2 from size because of both null terminators\n"
"	\n"
"	; resize this string\n"
"	push $r0			; store rhs\n"
"	push $r1\n"
"	call CString.Presize,I?size\n"
"	pop $r0				; restore rhs\n"
"	\n"
"	; copy other string into new buffer part\n"
"	ldw $r1, ($xr) 12	; reload this.size\n"
"	ldw $r2, ($xr) 20	; load this.buffer\n"
"	ldw $r3, ($r0) 12	; reload rhs.size\n"
"	ldw $r4, ($r0) 20	; load rhs.buffer\n"
"	dec $r1				; decrement size because of null terminator\n"
"	dec $r3				; decrement size because of null terminator\n"
"	add $r2, $r2, $r1	; add offset to buffer\n"
"	push $r3			; push buffer size\n"
"	push $r4			; push source buffer\n"
"	push $r2			; push destination buffer\n"
"	insc CopyMem		; copy string part\n"
"	\n"
"	.end:\n"
"	ret 1\n"
"\n"
"; String String.append(String this, Object rhs)\n"
"CString.Pappend,R@Object?rhs:\n"
"	ldw $cf, ($lb) -4	; load rhs\n"
"	je .end				; check if rhs = null\n"
"	\n"
"	; call rhs.toString()\n"
"	ldw $cf, ($cf) 8	; load rhs.vtable\n"
"	ldw $cf, ($cf) 20	; load rhs.vtable.toString\n"
"	call $cf			; call rhs.vtable.toString()\n"
"	\n"
"	; call this.append(String)\n"
"	push $ar			; store $ar\n"
"	push $ar			; push $ar as argument for this.append($ar)\n"
"	call CString.Pappend,R@String?rhs\n"
"	\n"
"	; decrement reference of temporary string from rhs.toString call\n"
"	pop $xr\n"
"	call dec_ref		; call rhs.dec_ref\n"
"	\n"
"	.end:\n"
"	ret 1\n"
"\n"
"; String String.append(String this, bool rhs)\n"
"CString.Pappend,B?rhs:\n"
"	; ...\n"
"	ret 1\n"
"\n"
"; String String.append(String this, int rhs)\n"
"CString.Pappend,I?rhs:\n"
"	; ...\n"
"	ret 1\n"
"\n"
"; String String.append(String this, float rhs)\n"
"CString.Pappend,F?rhs:\n"
"	; ...\n"
"	ret 1\n"
"\n"
"; String String.subString(String this, int pos, int len)\n"
"CString.PsubString,I?pos,I?len:\n"
"	; ...\n"
"	ret 2\n"
"\n"
"; void String.setChar(String this, int pos, int char)\n"
"CString.PsetChar,I?pos,I?char:\n"
"	; ...\n"
"	ret 2\n"
"\n"
"; int String.getChar(String this, int pos)\n"
"CString.PgetChar,I?pos:\n"
"	; ...\n"
"	mov $ar, 0 ; !!!\n"
"	ret 1\n"
"\n"
"; int String.pointer(Object this)\n"
"CString.Ppointer:\n"
"	ldw $ar, ($xr) 20	; load and return member this.buffer\n"
"	ret\n"
"\n"
"String.vtable:\n"
"	.word @CString.Prelease\n"
"	.word @CString.Pinit\n"
"	.word @CObject.PtypeID\n"
"	.word @CObject.PrefCount\n"
"	.word @CString.Pequals,R@Object?rhs\n"
"	.word @CString.PtoString\n"
"	.word @CString.Ppointer\n"
"	.word @CString.Pinit,I?size\n"
"	.word @CString.Pinit,R@String?source\n"
"	.word @CString.Pcopy\n"
"	.word @CString.Psize\n"
"	.word @CString.Presize,I?size\n"
"	.word @CString.Padjust\n"
"	.word @CString.Pempty\n"
"	.word @CString.Pappend,R@String?rhs\n"
"	.word @CString.Pappend,R@Object?rhs\n"
"	.word @CString.Pappend,B?rhs\n"
"	.word @CString.Pappend,I?rhs\n"
"	.word @CString.Pappend,F?rhs\n"
"	.word @CString.PsubString,I?pos,I?len\n"
"	.word @CString.PsetChar,I?pos,I?char\n"
"	.word @CString.PgetChar,I?pos\n"
"\n"
"\n"
"; ------- Array Class -------\n"
"; built-in array for objects\n"
"\n"
"; Array:\n"
";	Object	super	( 0)\n"
";	int		size	(12) <- number of entries\n"
";	int		bufSize	(16) <- actual number of entries\n"
";	int		buffer	(20)\n"
"\n"
"; void Array.release(Object this)\n"
"CArray.Prelease:\n"
"	; load object members\n"
"	ldw $r0, ($xr) 12	; load this.size\n"
"	ldw $r1, ($xr) 20	; load pointer to array buffer\n"
"	\n"
"	; de-reference all objects in array\n"
"	mov $xr, $r1		; set iterator to pointer: it := this.buffer\n"
"	sll $r0, $r0, 2		; calculate: size := this.size * 4\n"
"	add $r0, $r0, $xr	; determine end iterator: itEnd := it + size\n"
"	.loop:\n"
"	cmp $xr, $r0		; compare: it >= itEnd\n"
"	jge .end\n"
"		; store registers\n"
"		push $xr\n"
"		push $r0\n"
"		push $r1\n"
"		\n"
"		; decrement reference counter\n"
"		call dec_ref\n"
"		\n"
"		; restore registers\n"
"		pop $r1\n"
"		pop $r0\n"
"		pop $xr\n"
"	add $xr, $xr, 4		; it += 4\n"
"	jmp .loop\n"
"	.end:\n"
"	\n"
"	; free array buffer\n"
"	push $r1\n"
"	insc FreeMem		; delete array buffer\n"
"	ret\n"
"\n"
"; void Array.init(Object this)\n"
"CArray.Pinit:\n"
"; void PrimArray.init(Object this)\n"
"CPrimArray.Pinit:\n"
"	; this.size := 0\n"
"	mov $r0, 0\n"
"	stw $r0, ($xr) 12\n"
"	\n"
"	; initialize array buffer with 16 elements\n"
"	mov $r1, 16\n"
"	stw $r1, ($xr) 16\n"
"	\n"
"	; allocate array buffer\n"
"	push 64\n"
"	insc AllocMem		; allocate 16*4 bytes\n"
"	stw $ar, ($xr) 20	; store pointer to buffer in array instance\n"
"	ret\n"
"\n"
"; void Array.init(Array this, int size)\n"
"CArray.Pinit,I?size:\n"
"	; ...\n"
"	ret 1\n"
"\n"
"Array.vtable:\n"
"	.word @CArray.Prelease\n"
"	.word @CArray.Pinit\n"
"	.word @CObject.PtypeID\n"
"	.word @CObject.PrefCount\n"
";	.word @CArray.Pequals,R@Object?rhs\n"
";	.word @CArray.PtoString\n"
";	.word @CArray.Ppointer\n"
";	.word @CArray.Pinit,I?size\n"
";	.word @CArray.Pcopy\n"
";	.word @CArray.Psize\n"
";	.word @CArray.Presize,I?size\n"
";	.word @CArray.Pempty\n"
";	.word @CArray.Padd,R@Object?entry\n"
";	.word @CArray.Premove,I?pos\n"
"\n"
"\n"
"; ------- PrimArray Class -------\n"
"; built-in array for built-in types 'int' and 'float' and for all arrays of 'weak-references'\n"
"\n"
"; PrimArray:\n"
";	Object	super	( 0)\n"
";	int		size	(12) <- number of entries\n"
";	int		bufSize	(16) <- actual number of entries\n"
";	int		buffer	(20)\n"
"\n"
"; todo...\n"
"\n"
";PrimArray.vtable:\n"
";	.word @PrimArray.Prelease\n"
";	.word @PrimArray.Pinit\n"
";	.word @CObject.PtypeID\n"
";	.word @CObject.PrefCount\n"
";	.word @PrimArray.Pequals,R@Object?rhs\n"
";	.word @PrimArray.PtoString\n"
";	.word @PrimArray.Ppointer\n"
";	.word @PrimArray.Pinit,I?size\n"
";	.word @PrimArray.Pcopy\n"
";	.word @PrimArray.Psize\n"
";	.word @PrimArray.Presize,I?size\n"
";	.word @PrimArray.Pempty\n"
";	.word @PrimArray.Padd,R@Object?entry\n"
";	.word @PrimArray.Premove,I?pos\n"
"\n"
"\n"
"; ------- BoolArray Class -------\n"
"; built-in array for 'bool'\n"
"\n"
"; BoolArray:\n"
";	Object	super	( 0)\n"
";	int		size	(12) <- number of entries\n"
";	int		bufSize	(16) <- actual number of entries\n"
";	int		buffer	(20)\n"
"\n"
"; void BoolArray.release(Object this)\n"
"CBoolArray.Prelease:\n"
"	; free string buffer\n"
"	ldw $r0, ($xr) 20	; load pointer to array buffer\n"
"	push $r0\n"
"	insc FreeMem		; delete array buffer\n"
"	ret\n"
"\n"
"; void BoolArray.init(Object this)\n"
"CBoolArray.Pinit:\n"
"	; ...\n"
"	ret\n"
"\n"
"; void BoolArray.init(BoolArray this, int size)\n"
"CBoolArray.Pinit,I?size:\n"
"	; initialize array members\n"
"	ldw $cf, ($lb) -8		; load 'size'\n"
"	stw $cf, ($xr) 12		; write: this.size := size\n"
"	stw $cf, ($xr) 16		; write: this.bufSize := size\n"
"	\n"
"	; determine buffer size (in bytes): numBytes := (size + 7) >> 3\n"
"	add $cf, $cf, 7\n"
"	slr $cf, $cf, 3\n"
"	\n"
"	; allocate array buffer\n"
"	push $cf\n"
"	insc AllocMem			; allocate buffer\n"
"	stw $ar, ($xr) 20		; store pointer to buffer in array instance\n"
"	\n"
"	; initialize buffer with zeros\n"
"	mov $r0, 0\n"
"	.loop:\n"
"	jle .end				; compare: numBytes <= 0\n"
"		stw $r0, ($ar) 0	; store zero at (*buffer)\n"
"		inc $ar				; buffer++\n"
"	dec $cf					; numBytes--\n"
"	jmp .loop\n"
"	.end:\n"
"	ret 1\n"
"\n"
"BoolArray.vtable:\n"
"	.word @CBoolArray.Prelease\n"
"	.word @CBoolArray.Pinit\n"
"	.word @CObject.PtoString\n"
"	.word @CBoolArray.Pinit,I?size\n"
"\n"
"\n"
"; ------- Intrinsics Class -------\n"
"; built-in class for to access all VM intrinsics\n"
"\n"
"; int Intrinsics.allocMem(int size)\n"
"CIntrinsics.PallocMem,I?size:\n"
"	insc AllocMem\n"
"	ret\n"
"\n"
"; void Intrinsics.freeMem(int ptr)\n"
"CIntrinsics.PfreeMem,I?ptr:\n"
"	insc FreeMem\n"
"	ret\n"
"\n"
"; void Intrinsics.copyMem(int destPtr, int srcPtr, int size)\n"
"CIntrinsics.PcopyMem,I?destPtr,I?srcPtr,I?size:\n"
"	insc CopyMem\n"
"	ret\n"
"\n"
"; void Intrinsics.sysCall(int commandPtr)\n"
"CIntrinsics.PsysCall,I?commandPtr:\n"
"	insc SysCall\n"
"	ret\n"
"\n"
"; void Intrinsics.print(int textPtr)\n"
"CIntrinsics.Pprint,I?textPtr:\n"
"	insc Print\n"
"	ret\n"
"\n"
"; void Intrinsics.printLn(int textPtr)\n"
"CIntrinsics.PprintLn,I?textPtr:\n"
"	insc PrintLn\n"
"	ret\n"
"\n"
"; void Intrinsics.printInt(int value)\n"
"CIntrinsics.PprintInt,I?value:\n"
"	insc PrintInt\n"
"	ret\n"
"\n"
"; void Intrinsics.printFloat(float value)\n"
"CIntrinsics.PprintFloat,F?value:\n"
"	insc PrintFloat\n"
"	ret\n"
"\n"
"; void Intrinsics.input(int stringPtr, int maxLen)\n"
"CIntrinsics.Pinput,I?stringPtr,I?maxLen:\n"
"	insc Input\n"
"	ret\n"
"\n"
"; int Intrinsics.inputInt()\n"
"CIntrinsics.PinputInt:\n"
"	insc InputInt\n"
"	ret\n"
"\n"
"; float Intrinsics.inputFloat()\n"
"CIntrinsics.PinputFloat:\n"
"	insc InputFloat\n"
"	ret\n"
"\n"
"; int Intrinsics.createFile(int filenamePtr)\n"
"CIntrinsics.PcreateFile,I?filenamePtr:\n"
"	insc CreateFile\n"
"	ret\n"
"\n"
"; int Intrinsics.deleteFile(int filenamePtr)\n"
"CIntrinsics.PdeleteFile,I?filenamePtr:\n"
"	insc DeleteFile\n"
"	ret\n"
"\n"
"; int Intrinsics.openFile(int filenamePtr, int modePtr)\n"
"CIntrinsics.PopenFile,I?filenamePtr,I?modePtr:\n"
"	insc OpenFile\n"
"	ret\n"
"\n"
"; void Intrinsics.closeFile(int handlePtr)\n"
"CIntrinsics.PcloseFile,I?handlePtr:\n"
"	insc CloseFile\n"
"	ret\n"
"\n"
"; void Intrinsics.fileSetPos(int handlePtr, int offset, int origin)\n"
"CIntrinsics.PfileSetPos,I?handlePtr,I?offset,I?origin:\n"
"	insc FileSetPos\n"
"	ret\n"
"\n"
"; int Intrinsics.fileGetPos(int handlePtr)\n"
"CIntrinsics.PfileGetPos,I?handlePtr:\n"
"	insc FileGetPos\n"
"	ret\n"
"\n"
"; int Intrinsics.fileEOF(int handlePtr)\n"
"CIntrinsics.PfileEOF,I?handlePtr:\n"
"	insc FileEOF\n"
"	ret\n"
"\n"
"; void Intrinsics.writeByte(int handlePtr, int value)\n"
"CIntrinsics.PwriteByte,I?handlePtr,I?value:\n"
"	insc WriteByte\n"
"	ret\n"
"\n"
"; void Intrinsics.writeInt(int handlePtr, int value)\n"
"CIntrinsics.PwriteInt,I?handlePtr,I?value:\n"
"	insc WriteWord\n"
"	ret\n"
"\n"
"; void Intrinsics.writeFloat(int handlePtr, float value)\n"
"CIntrinsics.PwriteFloat,I?handlePtr,F?value:\n"
"	insc WriteWord\n"
"	ret\n"
"\n"
"; void Intrinsics.writeBuffer(int handlePtr, int bufferPtr, int size)\n"
"CIntrinsics.PwriteBuffer,I?handlePtr,I?bufferPtr,I?size:\n"
"	insc WriteBuffer\n"
"	ret\n"
"\n"
"; int Intrinsics.readByte(int handlePtr)\n"
"CIntrinsics.PreadByte,I?handlePtr:\n"
"	insc ReadByte\n"
"	ret\n"
"\n"
"; int Intrinsics.readInt(int handlePtr)\n"
"CIntrinsics.PreadInt,I?handlePtr:\n"
"	insc ReadWord\n"
"	ret\n"
"\n"
"; float Intrinsics.readFloat(int handlePtr)\n"
"CIntrinsics.PreadFloat,I?handlePtr:\n"
"	insc ReadWord\n"
"	ret\n"
"\n"
"; void Intrinsics.readBuffer(int handlePtr, int bufferPtr, int size)\n"
"CIntrinsics.PreadBuffer,I?handlePtr,I?bufferPtr,I?size:\n"
"	insc ReadBuffer\n"
"	ret\n"
"\n"
"; float Intrinsics.sin(float x)\n"
"CIntrinsics.Psin,F?x:\n"
"	insc Sin\n"
"	ret\n"
"\n"
"; float Intrinsics.cos(float x)\n"
"CIntrinsics.Pcos,F?x:\n"
"	insc Cos\n"
"	ret\n"
"\n"
"; float Intrinsics.tan(float x)\n"
"CIntrinsics.Ptan,F?x:\n"
"	insc Tan\n"
"	ret\n"
"\n"
"; float Intrinsics.asin(float x)\n"
"CIntrinsics.Pasin,F?x:\n"
"	insc ASin\n"
"	ret\n"
"\n"
"; float Intrinsics.acos(float x)\n"
"CIntrinsics.Pacos,F?x:\n"
"	insc ACos\n"
"	ret\n"
"\n"
"; float Intrinsics.atan(float x)\n"
"CIntrinsics.Patan,F?x:\n"
"	insc ATan\n"
"	ret\n"
"\n"
"; float Intrinsics.pow(float b, float e)\n"
"CIntrinsics.Ppow,F?b,F?e:\n"
"	insc Pow\n"
"	ret\n"
"\n"
"; float Intrinsics.log(float x)\n"
"CIntrinsics.Plog,F?x:\n"
"	insc Log\n"
"	ret\n"
"\n"
"; float Intrinsics.log2(float x)\n"
"CIntrinsics.Plog2,F?x:\n"
"	insc Log2\n"
"	ret\n"
"\n"
"; float Intrinsics.log10(float x)\n"
"CIntrinsics.Plog10,F?x:\n"
"	insc Log10\n"
"	ret\n"
"\n"
"; float Intrinsics.sqrt(float x)\n"
"CIntrinsics.Psqrt,F?x:\n"
"	insc Sqrt\n"
"	ret\n"
"\n"
"; int Intrinsics.randInt()\n"
"CIntrinsics.PrandInt:\n"
"	insc RandInt\n"
"	ret\n"
"\n"
"; float Intrinsics.randFloat()\n"
"CIntrinsics.PrandFloat:\n"
"	insc RandFloat\n"
"	ret\n"
"\n"
"; int Intrinsics.time()\n"
"CIntrinsics.Ptime:\n"
"	insc Time\n"
"	ret\n"
"\n"
"; void Intrinsics.sleep(int duration)\n"
"CIntrinsics.Psleep,I?duration:\n"
"	insc Sleep\n"
"	ret\n"
"\n"
"\n"
"\n"
