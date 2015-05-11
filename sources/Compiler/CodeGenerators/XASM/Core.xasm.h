"exit:\n"
"stop\n"
"new:\n"
"ldw $ar, ($lb) -4\n"
"push $ar\n"
"insc AllocMem\n"
"mov $r1, 1\n"
"stw $r1, ($ar) 0\n"
"ldw $r1, ($lb) -8\n"
"stw $r1, ($ar) 4\n"
"ldw $r1, ($lb) -12\n"
"stw $r1, ($ar) 8\n"
"ret 3\n"
"inc_ref:\n"
"mov $cf, $xr\n"
"je .end\n"
"ldw $r0, ($xr) 0\n"
"inc $r0\n"
"stw $r0, ($xr) 0\n"
".end:\n"
"ret\n"
"dec_ref:\n"
"mov $cf, $xr\n"
"je .end\n"
"ldw $r0, ($xr) 0\n"
"sub $cf, $r0, 1\n"
"je .rel\n"
"dec $r0\n"
"stw $r0, ($xr) 0\n"
"ret\n"
".rel:\n"
"ldw $r0, ($xr) 8\n"
"ldw $r0, ($r0) 0\n"
"push $xr\n"
"call $r0\n"
"insc FreeMem\n"
".end:\n"
"ret\n"
"dynamic_cast:\n"
"ldw $cf, ($lb) -4\n"
"je .end\n"
"mov $ar, $cf\n"
"ldw $r1, ($ar) 4\n"
"ldw $r2, ($lb) -8\n"
"ldw $r3, ($lb) -12\n"
"add $r3, $r3, $r2\n"
"cmp $r2, $r1\n"
"jg .end\n"
"cmp $r1, $r3\n"
"jg .end\n"
"ret 3\n"
".end:\n"
"mov $ar, 0\n"
"ret 3\n"
"resize_buffer:\n"
"ldw $r0, ($lb) -4\n"
"ldw $r1, ($xr) 16\n"
"cmp $r0, $r1\n"
"jle .else\n"
"ldw $r2, ($xr) 12\n"
"ldw $r3, ($xr) 20\n"
"sll $r4, $r0, 1\n"
"push $r4\n"
"insc AllocMem\n"
"stw $ar, ($xr) 20\n"
"stw $r4, ($xr) 16\n"
"push $r2\n"
"push $r3\n"
"push $ar\n"
"insc CopyMem\n"
"push $r3\n"
"insc FreeMem\n"
"push 0\n"
"sub $r3, $r0, $r2\n"
"push $r3\n"
"add $r3, $ar, $r2\n"
"push $r3\n"
"insc FillMem\n"
"jmp .end\n"
".else:\n"
"cmp $r1, 16\n"
"jle .end\n"
"slr $r1, $r1, 1\n"
"cmp $r0, $r1\n"
"jge .end\n"
"ldw $r2, ($xr) 12\n"
"ldw $r3, ($xr) 20\n"
"push $r0\n"
"insc AllocMem\n"
"stw $ar, ($xr) 20\n"
"stw $r4, ($xr) 16\n"
"push $r0\n"
"push $r3\n"
"push $ar\n"
"insc CopyMem\n"
"push $r3\n"
"insc FreeMem\n"
".end:\n"
"stw $r0, ($xr) 12\n"
"ret 1\n"
"String.copy_literal:\n"
"ldw $r0, ($lb) -4\n"
"push 24\n"
"insc AllocMem\n"
"mov $xr, $ar\n"
"push $r0\n"
"call CString.Pinit,R@String?source\n"
"mov $ar, $xr\n"
"ret 1\n"
"CObject.Prelease:\n"
"CObject.Pinit:\n"
"ret\n"
"CObject.PrefCount:\n"
"ldw $ar, ($xr) 0\n"
"ret\n"
"CObject.PtypeID:\n"
"ldw $ar, ($xr) 4\n"
"ret\n"
"CObject.Pequals,R@Object?rhs:\n"
"ldw $cf, ($lb) -4\n"
"je .false\n"
"ldw $r0, ($xr) 4\n"
"ldw $r1, ($cf) 4\n"
"cmp $r0, $r1\n"
"jne .false\n"
"mov $ar, 1\n"
"ret 1\n"
".false:\n"
"mov $ar, 0\n"
"ret 1\n"
"CObject.PtoString:\n"
"mov $ar, 0\n"
"ret\n"
"CObject.Ppointer:\n"
"add $ar, $xr, 12\n"
"ret\n"
"Object.vtable:\n"
".word @CObject.Prelease\n"
".word @CObject.Pinit\n"
".word @CObject.PtypeID\n"
".word @CObject.PrefCount\n"
".word @CObject.Pequals,R@Object?rhs\n"
".word @CObject.PtoString\n"
".word @CObject.Ppointer\n"
"CString.Prelease:\n"
"ldw $r0, ($xr) 20\n"
"push $r0\n"
"insc FreeMem\n"
"ret\n"
"CString.Pinit:\n"
"mov $r0, 1\n"
"stw $r0, ($xr) 12\n"
"mov $r1, 16\n"
"stw $r1, ($xr) 16\n"
"push 16\n"
"insc AllocMem\n"
"stw $ar, ($xr) 20\n"
"stw $r0, ($ar) 0\n"
"ret\n"
"CString.Pequals,R@Object?rhs:\n"
"ldw $cf, ($lb) -4\n"
"je .false\n"
"mov $tr, $cf\n"
"ldw $r0, ($xr) 4\n"
"ldw $r1, ($tr) 4\n"
"cmp $r0, $r1\n"
"jne .false\n"
"ldw $r0, ($xr) 12\n"
"ldw $r1, ($tr) 12\n"
"cmp $r0, $r1\n"
"jne .false\n"
"ldw $r2, ($xr) 20\n"
"ldw $r3, ($tr) 20\n"
".loop:\n"
"mov $cf, $r0\n"
"jle .break\n"
"ldw $r4, ($r2) 0\n"
"ldw $r5, ($r3) 0\n"
"cmp $r4, $r5\n"
"jne .false\n"
"inc $r2\n"
"inc $r3\n"
"dec $r0\n"
"jmp .loop\n"
".break:\n"
"mov $ar, 1\n"
"ret 1\n"
".false:\n"
"mov $ar, 0\n"
"ret 1\n"
"CString.Pinit,I?size:\n"
"call CString.Pinit\n"
"call CString.Presize,I?size\n"
"ret\n"
"CString.Pinit,I?char,I?count:\n"
"ret 2\n"
"CString.Pinit,R@String?source:\n"
"ldw $r0, ($lb) -4\n"
"push 20\n"
"push $r0\n"
"push $xr\n"
"insc CopyMem\n"
"ldw $r1, ($r0) 12\n"
"inc $r1\n"
"push $r1\n"
"insc AllocMem\n"
"stw $ar, ($xr) 20\n"
"ldw $r0, ($r0) 20\n"
"push $r1\n"
"push $r0\n"
"push $ar\n"
"insc CopyMem\n"
"ret 1\n"
"CString.PtoString:\n"
"call inc_ref\n"
"mov $ar, $xr\n"
"ret\n"
"CString.Ppointer:\n"
"CArray.Ppointer:\n"
"CIntArray.Ppointer:\n"
"CFloatArray.Ppointer:\n"
"CBoolArray.Ppointer:\n"
"CBuffer.Ppointer:\n"
"ldw $ar, ($xr) 20\n"
"ret\n"
"CString.Pcopy:\n"
"push @String.vtable\n"
"push 1\n"
"push 24\n"
"call new\n"
"pop $r1\n"
"ldw $r0, ($lb) -4\n"
"push $r1\n"
"push $r1\n"
"mov $xr, $r0\n"
"call CString.Pinit,R@String?source\n"
"pop $ar\n"
"ret\n"
"CString.Psize:\n"
"ldw $ar, ($xr) 12\n"
"dec $ar\n"
"ret\n"
"CString.Presize,I?size:\n"
"ldw $cf, ($lb) -4\n"
"jge .else\n"
"mov $cf, 0\n"
".else:\n"
"add $tr, $cf, 1\n"
"ldw $cf, ($xr) 12\n"
"cmp $cf, $tr\n"
"je .end\n"
"push $tr\n"
"call resize_buffer\n"
"ldw $r0, ($xr) 20\n"
"add $r0, $r0, $tr\n"
"mov $tr, 0\n"
"stw $tr, ($r0) -1\n"
".end:\n"
"ret 1\n"
"CString.Padjust:\n"
"ldw $r0, ($xr) 12\n"
"ldw $r1, ($xr) 20\n"
"mov $r2, 0\n"
".loop:\n"
"cmp $r2, $r0\n"
"jge .break\n"
"ldw $cf, ($r1) 0\n"
"je .break\n"
"inc $r2\n"
"inc $r1\n"
"jmp .loop\n"
".break:\n"
"push $r2\n"
"call CString.Presize,I?size\n"
"ret\n"
"CString.Pempty:\n"
"ldw $cf, ($xr) 12\n"
"dec $cf\n"
"jg .not_empty\n"
"mov $ar, 1\n"
"ret\n"
".not_empty:\n"
"mov $ar, 0\n"
"ret\n"
"CString.Pappend,R@String?rhs:\n"
"ldw $cf, ($lb) -4\n"
"je .end\n"
"mov $r0, $cf\n"
"ldw $r1, ($r0) 12\n"
"cmp $r1, 1\n"
"jle .end\n"
"ldw $r2, ($xr) 12\n"
"add $r1, $r1, $r2\n"
"sub $r1, $r1, 2\n"
"push $r2\n"
"push $r0\n"
"push $r1\n"
"call CString.Presize,I?size\n"
"pop $r0\n"
"pop $r1\n"
"ldw $r2, ($xr) 20\n"
"ldw $r3, ($r0) 12\n"
"ldw $r4, ($r0) 20\n"
"dec $r1\n"
"dec $r3\n"
"add $r2, $r2, $r1\n"
"push $r3\n"
"push $r4\n"
"push $r2\n"
"insc CopyMem\n"
".end:\n"
"mov $ar, $xr\n"
"ret 1\n"
"CString.Pappend,R@Object?rhs:\n"
"ldw $cf, ($lb) -4\n"
"je .end\n"
"ldw $cf, ($cf) 8\n"
"ldw $cf, ($cf) 20\n"
"call $cf\n"
"push $ar\n"
"push $ar\n"
"call CString.Pappend,R@String?rhs\n"
"pop $xr\n"
"call dec_ref\n"
".end:\n"
"ret 1\n"
"CString.Pappend,B?rhs:\n"
"ret 1\n"
"CString.Pappend,I?rhs:\n"
"ret 1\n"
"CString.Pappend,F?rhs:\n"
"ret 1\n"
"CString.PsubString,I?from,I?length:\n"
"mov $ar, 0\n"
"ret 2\n"
"CString.Pfind,R@String?search,I?from:\n"
"mov $ar, -1\n"
"ret 2\n"
"CString.PsetChar,I?pos,I?char:\n"
"ret 2\n"
"CString.PgetChar,I?pos:\n"
"mov $ar, 0\n"
"ret 1\n"
"String.vtable:\n"
".word @CString.Prelease\n"
".word @CString.Pinit\n"
".word @CObject.PtypeID\n"
".word @CObject.PrefCount\n"
".word @CString.Pequals,R@Object?rhs\n"
".word @CString.PtoString\n"
".word @CString.Ppointer\n"
".word @CString.Pinit,I?size\n"
".word @CString.Pinit,I?char,I?count\n"
".word @CString.Pinit,R@String?source\n"
".word @CString.Pcopy\n"
".word @CString.Psize\n"
".word @CString.Presize,I?size\n"
".word @CString.Padjust\n"
".word @CString.Pempty\n"
".word @CString.Pappend,R@String?rhs\n"
".word @CString.Pappend,R@Object?rhs\n"
".word @CString.Pappend,B?rhs\n"
".word @CString.Pappend,I?rhs\n"
".word @CString.Pappend,F?rhs\n"
".word @CString.PsubString,I?from,I?length\n"
".word @CString.Pfind,R@String?search,I?from\n"
".word @CString.PsetChar,I?pos,I?char\n"
".word @CString.PgetChar,I?pos\n"
"CArray.Prelease:\n"
"ldw $r0, ($xr) 12\n"
"ldw $r1, ($xr) 20\n"
"mov $xr, $r1\n"
"sll $r0, $r0, 2\n"
"add $r0, $r0, $xr\n"
".loop:\n"
"cmp $xr, $r0\n"
"jge .break\n"
"push $xr\n"
"push $r0\n"
"push $r1\n"
"call dec_ref\n"
"pop $r1\n"
"pop $r0\n"
"pop $xr\n"
"add $xr, $xr, 4\n"
"jmp .loop\n"
".break:\n"
"push $r1\n"
"insc FreeMem\n"
"ret\n"
"CArray.Pinit:\n"
"CIntArray.Pinit:\n"
"CFloatArray.Pinit:\n"
"CBoolArray.Pinit:\n"
"CBuffer.Pinit:\n"
"mov $r0, 0\n"
"stw $r0, ($xr) 12\n"
"mov $r1, 64\n"
"stw $r1, ($xr) 16\n"
"push 64\n"
"insc AllocMem\n"
"stw $ar, ($xr) 20\n"
"ret\n"
"CArray.Pinit,I?size:\n"
"CIntArray.Pinit,I?size:\n"
"CFloatArray.Pinit,I?size:\n"
"ret 1\n"
"CArray.Pequals,R@Object?rhs:\n"
"ret 1\n"
"CArray.PtoString:\n"
"ret\n"
"CArray.Pcopy:\n"
"CIntArray.Pcopy:\n"
"CFloatArray.Pcopy:\n"
"ret\n"
"CArray.Psize:\n"
"CIntArray.Psize:\n"
"CFloatArray.Psize:\n"
"ldw $ar, ($xr) 12\n"
"slr $ar, $ar, 2\n"
"ret\n"
"CArray.Presize,I?size:\n"
"CIntArray.Presize,I?size:\n"
"CFloatArray.Presize,I?size:\n"
"ldw $cf, ($lb) -4\n"
"jge .else\n"
"mov $cf, 0\n"
".else:\n"
"sll $tr, $cf, 2\n"
"ldw $cf, ($xr) 12\n"
"cmp $cf, $tr\n"
"je .end\n"
"push $tr\n"
"call resize_buffer\n"
".end:\n"
"ret 1\n"
"CArray.Pempty:\n"
"CIntArray.Pempty:\n"
"CFloatArray.Pempty:\n"
"ldw $cf, ($xr) 12\n"
"jg .not_empty\n"
"mov $ar, 1\n"
"ret\n"
".not_empty:\n"
"mov $ar, 0\n"
"ret\n"
"CArray.Pappend,R@Object?entry:\n"
"ret 1\n"
"CArray.Pinsert,R@Object?entry,I?pos:\n"
"ret 2\n"
"CArray.Premove,I?pos:\n"
"CIntArray.Premove,I?pos:\n"
"CFloatArray.Premove,I?pos:\n"
"ret 1\n"
"CArray.Pfind,R@Object?entry,I?from:\n"
"ret 2\n"
"CArray.Pcontains,R@Object?entry:\n"
"ret 1\n"
"Array.vtable:\n"
".word @CArray.Prelease\n"
".word @CArray.Pinit\n"
".word @CObject.PtypeID\n"
".word @CObject.PrefCount\n"
".word @CArray.Pequals,R@Object?rhs\n"
".word @CArray.PtoString\n"
".word @CArray.Ppointer\n"
".word @CArray.Pinit,I?size\n"
".word @CArray.Pcopy\n"
".word @CArray.Psize\n"
".word @CArray.Presize,I?size\n"
".word @CArray.Pempty\n"
".word @CArray.Pappend,R@Object?entry\n"
".word @CArray.Pinsert,R@Object?entry,I?pos\n"
".word @CArray.Premove,I?pos\n"
".word @CArray.Pfind,R@Object?entry,I?from\n"
".word @CArray.Pcontains,R@Object?entry\n"
"CIntArray.Prelease:\n"
"CFloatArray.Prelease:\n"
"CBoolArray.Prelease:\n"
"CBuffer.Prelease:\n"
"ldw $r0, ($xr) 20\n"
"push $r0\n"
"insc FreeMem\n"
"ret\n"
"CIntArray.Pequals,R@Object?rhs:\n"
"ret 1\n"
"CIntArray.PtoString:\n"
"ret\n"
"CIntArray.Pappend,I?entry:\n"
"ret 1\n"
"CIntArray.Pinsert,I?entry,I?pos:\n"
"ret 2\n"
"CIntArray.Pfind,I?entry,I?from:\n"
"ret 2\n"
"CIntArray.Pcontains,I?entry:\n"
"ret 1\n"
"IntArray.vtable:\n"
".word @CIntArray.Prelease\n"
".word @CIntArray.Pinit\n"
".word @CObject.PtypeID\n"
".word @CObject.PrefCount\n"
".word @CIntArray.Pequals,R@Object?rhs\n"
".word @CIntArray.PtoString\n"
".word @CIntArray.Ppointer\n"
".word @CIntArray.Pinit,I?size\n"
".word @CIntArray.Pcopy\n"
".word @CIntArray.Psize\n"
".word @CIntArray.Presize,I?size\n"
".word @CIntArray.Pempty\n"
".word @CIntArray.Pappend,I?entry\n"
".word @CIntArray.Pinsert,I?entry,I?pos\n"
".word @CIntArray.Premove,I?pos\n"
".word @CIntArray.Pfind,I?entry,I?from\n"
".word @CIntArray.Pcontains,I?entry\n"
"CFloatArray.Pequals,R@Object?rhs:\n"
"ret 1\n"
"CFloatArray.PtoString:\n"
"ret\n"
"CFloatArray.Pappend,F?entry:\n"
"ret 1\n"
"CFloatArray.Pinsert,F?entry,I?pos:\n"
"ret 2\n"
"CFloatArray.Pfind,F?entry,I?from:\n"
"ret 2\n"
"CFloatArray.Pcontains,F?entry:\n"
"ret 1\n"
"FloatArray.vtable:\n"
".word @CFloatArray.Prelease\n"
".word @CFloatArray.Pinit\n"
".word @CObject.PtypeID\n"
".word @CObject.PrefCount\n"
".word @CFloatArray.Pequals,R@Object?rhs\n"
".word @CFloatArray.PtoString\n"
".word @CFloatArray.Ppointer\n"
".word @CFloatArray.Pinit,I?size\n"
".word @CFloatArray.Pcopy\n"
".word @CFloatArray.Psize\n"
".word @CFloatArray.Presize,I?size\n"
".word @CFloatArray.Pempty\n"
".word @CFloatArray.Pappend,F?entry\n"
".word @CFloatArray.Pinsert,F?entry,I?pos\n"
".word @CFloatArray.Premove,I?pos\n"
".word @CFloatArray.Pfind,F?entry,I?from\n"
".word @CFloatArray.Pcontains,F?entry\n"
"CBoolArray.Pinit,I?size:\n"
"ldw $cf, ($lb) -8\n"
"stw $cf, ($xr) 12\n"
"stw $cf, ($xr) 16\n"
"add $cf, $cf, 7\n"
"slr $cf, $cf, 3\n"
"push $cf\n"
"insc AllocMem\n"
"stw $ar, ($xr) 20\n"
"mov $r0, 0\n"
".loop:\n"
"jle .break\n"
"stw $r0, ($ar) 0\n"
"inc $ar\n"
"dec $cf\n"
"jmp .loop\n"
".break:\n"
"ret 1\n"
"BoolArray.vtable:\n"
".word @CBoolArray.Prelease\n"
".word @CBoolArray.Pinit\n"
".word @CObject.PtoString\n"
".word @CBoolArray.Pinit,I?size\n"
"CBuffer.Pequals,R@Object?rhs:\n"
"ret 1\n"
"CBuffer.Pinit,I?size,I?fill:\n"
"ldw $cf, ($lb) -4\n"
"jge .gt_zero\n"
"mov $cf, 0\n"
".gt_zero:\n"
"stw $cf, ($xr) 12\n"
"mov $r0, $cf\n"
"cmp $r0, 64\n"
"jge .gt_64\n"
"mov $r0, 64\n"
".gt_64:\n"
"stw $r0, ($xr) 16\n"
"push $r0\n"
"insc AllocMem\n"
"stw $ar, ($xr) 20\n"
"ldw $r1, ($lb) -8\n"
"push $r1\n"
"push $r0\n"
"push $ar\n"
"insc FillMem\n"
"ret 2\n"
"CBuffer.Pcopy:\n"
"ret\n"
"CBuffer.Psize:\n"
"ldw $ar, ($xr) 12\n"
"ret\n"
"CBuffer.Presize,I?size:\n"
"ldw $cf, ($lb) -4\n"
"jge .else\n"
"mov $cf, 0\n"
".else:\n"
"mov $tr, $cf\n"
"ldw $cf, ($xr) 12\n"
"cmp $cf, $tr\n"
"je .end\n"
"push $tr\n"
"call resize_buffer\n"
".end:\n"
"ret 1\n"
"CBuffer.PwriteByte,I?offset,I?value:\n"
"ret 2\n"
"CBuffer.PreadByte,I?offset:\n"
"ret 1\n"
"CBuffer.PwriteInt,I?offset,I?value:\n"
"ret 2\n"
"CBuffer.PreadInt,I?offset:\n"
"ret 1\n"
"CBuffer.PwriteFloat,I?offset,F?value:\n"
"ret 2\n"
"CBuffer.PreadFloat,I?offset:\n"
"ret 1\n"
"CBuffer.PwriteBuffer,I?offset,I?size,R@Buffer?buffer:\n"
"ret 3\n"
"CBuffer.PreadBuffer,I?offset,I?size,R@Buffer?buffer:\n"
"ret 3\n"
"Buffer.vtable:\n"
".word @CBuffer.Prelease\n"
".word @CBuffer.Pinit\n"
".word @CObject.PtypeID\n"
".word @CObject.PrefCount\n"
".word @CBuffer.Pequals,R@Object?rhs\n"
".word @CObject.PtoString\n"
".word @CBuffer.Ppointer\n"
".word @CBuffer.Pinit,I?size,I?fill\n"
".word @CBuffer.Pcopy\n"
".word @CBuffer.Psize\n"
".word @CBuffer.Presize,I?size\n"
".word @CBuffer.PwriteByte,I?offset,I?value\n"
".word @CBuffer.PreadByte,I?offset\n"
".word @CBuffer.PwriteInt,I?offset,I?value\n"
".word @CBuffer.PreadInt,I?offset\n"
".word @CBuffer.PwriteFloat,I?offset,F?value\n"
".word @CBuffer.PreadFloat,I?offset\n"
".word @CBuffer.PwriteBuffer,I?offset,I?size,R@Buffer?buffer\n"
".word @CBuffer.PreadBuffer,I?offset,I?size,R@Buffer?buffer\n"
"CIntrinsics.PallocMem,I?size:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc AllocMem\n"
"ret 1\n"
"CIntrinsics.PfreeMem,I?ptr:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc FreeMem\n"
"ret 1\n"
"CIntrinsics.PcopyMem,I?destPtr,I?srcPtr,I?size:\n"
"ldw $tr, ($lb) -12\n"
"push $tr\n"
"ldw $tr, ($lb) -8\n"
"push $tr\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc CopyMem\n"
"ret 3\n"
"CIntrinsics.PsysCall,I?commandPtr:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc SysCall\n"
"ret 1\n"
"CIntrinsics.Pprint,I?textPtr:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc Print\n"
"ret 1\n"
"CIntrinsics.PprintLn,I?textPtr:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc PrintLn\n"
"ret 1\n"
"CIntrinsics.PprintInt,I?value:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc PrintInt\n"
"ret 1\n"
"CIntrinsics.PprintFloat,F?value:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc PrintFloat\n"
"ret 1\n"
"CIntrinsics.Pinput,I?stringPtr,I?maxLen:\n"
"ldw $tr, ($lb) -8\n"
"push $tr\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc Input\n"
"ret 2\n"
"CIntrinsics.PinputInt:\n"
"insc InputInt\n"
"ret\n"
"CIntrinsics.PinputFloat:\n"
"insc InputFloat\n"
"ret\n"
"CIntrinsics.PcreateFile,I?filenamePtr:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc CreateFile\n"
"ret 1\n"
"CIntrinsics.PdeleteFile,I?filenamePtr:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc DeleteFile\n"
"ret 1\n"
"CIntrinsics.PopenFile,I?filenamePtr,I?modePtr:\n"
"ldw $tr, ($lb) -8\n"
"push $tr\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc OpenFile\n"
"ret 2\n"
"CIntrinsics.PcloseFile,I?handlePtr:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc CloseFile\n"
"ret 1\n"
"CIntrinsics.PfileSetPos,I?handlePtr,I?offset,I?origin:\n"
"ldw $tr, ($lb) -12\n"
"push $tr\n"
"ldw $tr, ($lb) -8\n"
"push $tr\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc FileSetPos\n"
"ret 3\n"
"CIntrinsics.PfileGetPos,I?handlePtr:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc FileGetPos\n"
"ret 1\n"
"CIntrinsics.PfileEOF,I?handlePtr:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc FileEOF\n"
"ret 1\n"
"CIntrinsics.PwriteByte,I?handlePtr,I?value:\n"
"ldw $tr, ($lb) -8\n"
"push $tr\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc WriteByte\n"
"ret 2\n"
"CIntrinsics.PwriteInt,I?handlePtr,I?value:\n"
"ldw $tr, ($lb) -8\n"
"push $tr\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc WriteWord\n"
"ret 2\n"
"CIntrinsics.PwriteFloat,I?handlePtr,F?value:\n"
"ldw $tr, ($lb) -8\n"
"push $tr\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc WriteWord\n"
"ret 2\n"
"CIntrinsics.PwriteBuffer,I?handlePtr,I?bufferPtr,I?size:\n"
"ldw $tr, ($lb) -12\n"
"push $tr\n"
"ldw $tr, ($lb) -8\n"
"push $tr\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc WriteBuffer\n"
"ret 3\n"
"CIntrinsics.PreadByte,I?handlePtr:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc ReadByte\n"
"ret 1\n"
"CIntrinsics.PreadInt,I?handlePtr:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc ReadWord\n"
"ret 1\n"
"CIntrinsics.PreadFloat,I?handlePtr:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc ReadWord\n"
"ret 1\n"
"CIntrinsics.PreadBuffer,I?handlePtr,I?bufferPtr,I?size:\n"
"ldw $tr, ($lb) -12\n"
"push $tr\n"
"ldw $tr, ($lb) -8\n"
"push $tr\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc ReadBuffer\n"
"ret 3\n"
"CIntrinsics.Ptime:\n"
"insc Time\n"
"ret\n"
"CIntrinsics.Psleep,I?duration:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc Sleep\n"
"ret 1\n"
"CMath.Psin,F?x:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc Sin\n"
"ret 1\n"
"CMath.Pcos,F?x:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc Cos\n"
"ret 1\n"
"CMath.Ptan,F?x:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc Tan\n"
"ret 1\n"
"CMath.Pasin,F?x:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc ASin\n"
"ret 1\n"
"CMath.Pacos,F?x:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc ACos\n"
"ret 1\n"
"CMath.Patan,F?x:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc ATan\n"
"ret 1\n"
"CMath.Ppow,F?b,F?e:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc Pow\n"
"ret 1\n"
"CMath.Plog,F?x:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc Log\n"
"ret 1\n"
"CMath.Plog2,F?x:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc Log2\n"
"ret 1\n"
"CMath.Plog10,F?x:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc Log10\n"
"ret 1\n"
"CMath.Psqrt,F?x:\n"
"ldw $tr, ($lb) -4\n"
"push $tr\n"
"insc Sqrt\n"
"ret 1\n"
"CMath.PrandInt:\n"
"insc RandInt\n"
"ret\n"
"CMath.PrandFloat:\n"
"insc RandFloat\n"
"ret\n"
