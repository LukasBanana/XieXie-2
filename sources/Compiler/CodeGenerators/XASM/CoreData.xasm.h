"Object.vtable:\n"
".word @CObject.Prelease\n"
".word @CObject.Pinit\n"
".word @CObject.PtypeID\n"
".word @CObject.PrefCount\n"
".word @CObject.Pequals,R@Object?rhs\n"
".word @CObject.PtoString\n"
".word @CObject.Ppointer\n"
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
".word @CString.Pinit,R@String?source,I?from,I?length\n"
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
"BoolArray.vtable:\n"
".word @CBoolArray.Prelease\n"
".word @CBoolArray.Pinit\n"
".word @CObject.PtoString\n"
".word @CBoolArray.Pinit,I?size\n"
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
