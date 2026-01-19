	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 15, 0	sdk_version 15, 5
	.intel_syntax noprefix
	.file	0 "/Users/timuradiatullin/narnia/spbpu/sem3/cvm/lab6" "00.c" md5 0x0e0c2c7068837938eb584176c9a7436e
	.globl	_main                           ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
Lfunc_begin0:
	.loc	0 4 0                           ## 00.c:4:0
	.cfi_startproc
## %bb.0:
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset ebp, -8
	mov	ebp, esp
	.cfi_def_cfa_register ebp
	push	eax
	mov	dword ptr [ebp - 4], 0
Ltmp0:
	.loc	0 5 9 prologue_end              ## 00.c:5:9
	mov	dword ptr [_i], 0
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
Ltmp1:
	.loc	0 5 16 is_stmt 0                ## 00.c:5:16
	cmp	dword ptr [_i], 9
Ltmp2:
	.loc	0 5 2                           ## 00.c:5:2
	jae	LBB0_7
## %bb.2:                               ##   in Loop: Header=BB0_1 Depth=1
Ltmp3:
	.loc	0 6 9 is_stmt 1                 ## 00.c:6:9
	cmp	dword ptr [_i], 6
Ltmp4:
	.loc	0 6 7 is_stmt 0                 ## 00.c:6:7
	je	LBB0_4
## %bb.3:                               ##   in Loop: Header=BB0_1 Depth=1
Ltmp5:
	.loc	0 7 21 is_stmt 1                ## 00.c:7:21
	imul	edx, dword ptr [_i], 17
	.loc	0 7 25 is_stmt 0                ## 00.c:7:25
	and	edx, 14
	.loc	0 7 6                           ## 00.c:7:6
	mov	eax, dword ptr [_pMas]
	.loc	0 7 13                          ## 00.c:7:13
	mov	ecx, dword ptr [_i]
	.loc	0 7 16                          ## 00.c:7:16
	mov	dword ptr [eax + 4*ecx], edx
	.loc	0 7 4                           ## 00.c:7:4
	jmp	LBB0_5
LBB0_4:                                 ##   in Loop: Header=BB0_1 Depth=1
Ltmp6:
	.loc	0 9 24 is_stmt 1                ## 00.c:9:24
	imul	edx, dword ptr [_i], 26
	.loc	0 9 27 is_stmt 0                ## 00.c:9:27
	shr	edx, 2
	.loc	0 9 7                           ## 00.c:9:7
	mov	eax, dword ptr [_pMas]
	.loc	0 9 14                          ## 00.c:9:14
	mov	ecx, dword ptr [_i]
	.loc	0 9 17                          ## 00.c:9:17
	mov	dword ptr [eax + 4*ecx], edx
Ltmp7:
LBB0_5:                                 ##   in Loop: Header=BB0_1 Depth=1
	.loc	0 11 2 is_stmt 1                ## 00.c:11:2
	jmp	LBB0_6
Ltmp8:
LBB0_6:                                 ##   in Loop: Header=BB0_1 Depth=1
	.loc	0 5 22                          ## 00.c:5:22
	mov	eax, dword ptr [_i]
	add	eax, 1
	mov	dword ptr [_i], eax
	.loc	0 5 2 is_stmt 0                 ## 00.c:5:2
	jmp	LBB0_1
Ltmp9:
LBB0_7:
	.loc	0 12 3 is_stmt 1                ## 00.c:12:3
	xor	eax, eax
	.loc	0 12 3 epilogue_begin is_stmt 0 ## 00.c:12:3
	add	esp, 4
	pop	ebp
	ret
Ltmp10:
Lfunc_end0:
	.cfi_endproc
                                        ## -- End function
	.comm	_Mas,40,2                       ## @Mas
	.section	__DATA,__data
	.globl	_pMas                           ## @pMas
	.p2align	2, 0x0
_pMas:
	.long	_Mas

	.comm	_i,4,2                          ## @i
	.section	__DWARF,__debug_abbrev,regular,debug
Lsection_abbrev:
	.byte	1                               ## Abbreviation Code
	.byte	17                              ## DW_TAG_compile_unit
	.byte	1                               ## DW_CHILDREN_yes
	.byte	37                              ## DW_AT_producer
	.byte	37                              ## DW_FORM_strx1
	.byte	19                              ## DW_AT_language
	.byte	5                               ## DW_FORM_data2
	.byte	3                               ## DW_AT_name
	.byte	37                              ## DW_FORM_strx1
	.ascii	"\202|"                         ## DW_AT_LLVM_sysroot
	.byte	37                              ## DW_FORM_strx1
	.ascii	"\357\177"                      ## DW_AT_APPLE_sdk
	.byte	37                              ## DW_FORM_strx1
	.byte	114                             ## DW_AT_str_offsets_base
	.byte	23                              ## DW_FORM_sec_offset
	.byte	16                              ## DW_AT_stmt_list
	.byte	23                              ## DW_FORM_sec_offset
	.byte	27                              ## DW_AT_comp_dir
	.byte	37                              ## DW_FORM_strx1
	.byte	17                              ## DW_AT_low_pc
	.byte	27                              ## DW_FORM_addrx
	.byte	18                              ## DW_AT_high_pc
	.byte	6                               ## DW_FORM_data4
	.byte	115                             ## DW_AT_addr_base
	.byte	23                              ## DW_FORM_sec_offset
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	2                               ## Abbreviation Code
	.byte	52                              ## DW_TAG_variable
	.byte	0                               ## DW_CHILDREN_no
	.byte	3                               ## DW_AT_name
	.byte	37                              ## DW_FORM_strx1
	.byte	73                              ## DW_AT_type
	.byte	19                              ## DW_FORM_ref4
	.byte	63                              ## DW_AT_external
	.byte	25                              ## DW_FORM_flag_present
	.byte	58                              ## DW_AT_decl_file
	.byte	11                              ## DW_FORM_data1
	.byte	59                              ## DW_AT_decl_line
	.byte	11                              ## DW_FORM_data1
	.byte	2                               ## DW_AT_location
	.byte	24                              ## DW_FORM_exprloc
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	3                               ## Abbreviation Code
	.byte	15                              ## DW_TAG_pointer_type
	.byte	0                               ## DW_CHILDREN_no
	.byte	73                              ## DW_AT_type
	.byte	19                              ## DW_FORM_ref4
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	4                               ## Abbreviation Code
	.byte	36                              ## DW_TAG_base_type
	.byte	0                               ## DW_CHILDREN_no
	.byte	3                               ## DW_AT_name
	.byte	37                              ## DW_FORM_strx1
	.byte	62                              ## DW_AT_encoding
	.byte	11                              ## DW_FORM_data1
	.byte	11                              ## DW_AT_byte_size
	.byte	11                              ## DW_FORM_data1
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	5                               ## Abbreviation Code
	.byte	1                               ## DW_TAG_array_type
	.byte	1                               ## DW_CHILDREN_yes
	.byte	73                              ## DW_AT_type
	.byte	19                              ## DW_FORM_ref4
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	6                               ## Abbreviation Code
	.byte	33                              ## DW_TAG_subrange_type
	.byte	0                               ## DW_CHILDREN_no
	.byte	73                              ## DW_AT_type
	.byte	19                              ## DW_FORM_ref4
	.byte	55                              ## DW_AT_count
	.byte	11                              ## DW_FORM_data1
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	7                               ## Abbreviation Code
	.byte	36                              ## DW_TAG_base_type
	.byte	0                               ## DW_CHILDREN_no
	.byte	3                               ## DW_AT_name
	.byte	37                              ## DW_FORM_strx1
	.byte	11                              ## DW_AT_byte_size
	.byte	11                              ## DW_FORM_data1
	.byte	62                              ## DW_AT_encoding
	.byte	11                              ## DW_FORM_data1
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	8                               ## Abbreviation Code
	.byte	46                              ## DW_TAG_subprogram
	.byte	0                               ## DW_CHILDREN_no
	.byte	17                              ## DW_AT_low_pc
	.byte	27                              ## DW_FORM_addrx
	.byte	18                              ## DW_AT_high_pc
	.byte	6                               ## DW_FORM_data4
	.byte	64                              ## DW_AT_frame_base
	.byte	24                              ## DW_FORM_exprloc
	.byte	3                               ## DW_AT_name
	.byte	37                              ## DW_FORM_strx1
	.byte	58                              ## DW_AT_decl_file
	.byte	11                              ## DW_FORM_data1
	.byte	59                              ## DW_AT_decl_line
	.byte	11                              ## DW_FORM_data1
	.byte	73                              ## DW_AT_type
	.byte	19                              ## DW_FORM_ref4
	.byte	63                              ## DW_AT_external
	.byte	25                              ## DW_FORM_flag_present
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	0                               ## EOM(3)
	.section	__DWARF,__debug_info,regular,debug
Lsection_info:
Lcu_begin0:
.set Lset0, Ldebug_info_end0-Ldebug_info_start0 ## Length of Unit
	.long	Lset0
Ldebug_info_start0:
	.short	5                               ## DWARF version number
	.byte	1                               ## DWARF Unit Type
	.byte	4                               ## Address Size (in bytes)
.set Lset1, Lsection_abbrev-Lsection_abbrev ## Offset Into Abbrev. Section
	.long	Lset1
	.byte	1                               ## Abbrev [1] 0xc:0x67 DW_TAG_compile_unit
	.byte	0                               ## DW_AT_producer
	.short	29                              ## DW_AT_language
	.byte	1                               ## DW_AT_name
	.byte	2                               ## DW_AT_LLVM_sysroot
	.byte	3                               ## DW_AT_APPLE_sdk
.set Lset2, Lstr_offsets_base0-Lsection_str_off ## DW_AT_str_offsets_base
	.long	Lset2
.set Lset3, Lline_table_start0-Lsection_line ## DW_AT_stmt_list
	.long	Lset3
	.byte	4                               ## DW_AT_comp_dir
	.byte	3                               ## DW_AT_low_pc
.set Lset4, Lfunc_end0-Lfunc_begin0     ## DW_AT_high_pc
	.long	Lset4
.set Lset5, Laddr_table_base0-Lsection_info0 ## DW_AT_addr_base
	.long	Lset5
	.byte	2                               ## Abbrev [2] 0x25:0xb DW_TAG_variable
	.byte	5                               ## DW_AT_name
	.long	48                              ## DW_AT_type
                                        ## DW_AT_external
	.byte	0                               ## DW_AT_decl_file
	.byte	3                               ## DW_AT_decl_line
	.byte	2                               ## DW_AT_location
	.byte	161
	.byte	0
	.byte	3                               ## Abbrev [3] 0x30:0x5 DW_TAG_pointer_type
	.long	53                              ## DW_AT_type
	.byte	4                               ## Abbrev [4] 0x35:0x4 DW_TAG_base_type
	.byte	6                               ## DW_AT_name
	.byte	5                               ## DW_AT_encoding
	.byte	4                               ## DW_AT_byte_size
	.byte	2                               ## Abbrev [2] 0x39:0xb DW_TAG_variable
	.byte	7                               ## DW_AT_name
	.long	68                              ## DW_AT_type
                                        ## DW_AT_external
	.byte	0                               ## DW_AT_decl_file
	.byte	1                               ## DW_AT_decl_line
	.byte	2                               ## DW_AT_location
	.byte	161
	.byte	1
	.byte	5                               ## Abbrev [5] 0x44:0xc DW_TAG_array_type
	.long	53                              ## DW_AT_type
	.byte	6                               ## Abbrev [6] 0x49:0x6 DW_TAG_subrange_type
	.long	80                              ## DW_AT_type
	.byte	10                              ## DW_AT_count
	.byte	0                               ## End Of Children Mark
	.byte	7                               ## Abbrev [7] 0x50:0x4 DW_TAG_base_type
	.byte	8                               ## DW_AT_name
	.byte	8                               ## DW_AT_byte_size
	.byte	7                               ## DW_AT_encoding
	.byte	2                               ## Abbrev [2] 0x54:0xb DW_TAG_variable
	.byte	9                               ## DW_AT_name
	.long	95                              ## DW_AT_type
                                        ## DW_AT_external
	.byte	0                               ## DW_AT_decl_file
	.byte	2                               ## DW_AT_decl_line
	.byte	2                               ## DW_AT_location
	.byte	161
	.byte	2
	.byte	4                               ## Abbrev [4] 0x5f:0x4 DW_TAG_base_type
	.byte	10                              ## DW_AT_name
	.byte	7                               ## DW_AT_encoding
	.byte	4                               ## DW_AT_byte_size
	.byte	8                               ## Abbrev [8] 0x63:0xf DW_TAG_subprogram
	.byte	3                               ## DW_AT_low_pc
.set Lset6, Lfunc_end0-Lfunc_begin0     ## DW_AT_high_pc
	.long	Lset6
	.byte	1                               ## DW_AT_frame_base
	.byte	85
	.byte	11                              ## DW_AT_name
	.byte	0                               ## DW_AT_decl_file
	.byte	4                               ## DW_AT_decl_line
	.long	53                              ## DW_AT_type
                                        ## DW_AT_external
	.byte	0                               ## End Of Children Mark
Ldebug_info_end0:
	.section	__DWARF,__debug_str_offs,regular,debug
Lsection_str_off:
	.long	52                              ## Length of String Offsets Set
	.short	5
	.short	0
Lstr_offsets_base0:
	.section	__DWARF,__debug_str,regular,debug
Linfo_string:
	.asciz	"Apple clang version 17.0.0 (clang-1700.0.13.5)" ## string offset=0
	.asciz	"00.c"                          ## string offset=47
	.asciz	"/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk" ## string offset=52
	.asciz	"MacOSX.sdk"                    ## string offset=104
	.asciz	"/Users/timuradiatullin/narnia/spbpu/sem3/cvm/lab6" ## string offset=115
	.asciz	"pMas"                          ## string offset=165
	.asciz	"int"                           ## string offset=170
	.asciz	"Mas"                           ## string offset=174
	.asciz	"__ARRAY_SIZE_TYPE__"           ## string offset=178
	.asciz	"i"                             ## string offset=198
	.asciz	"unsigned long"                 ## string offset=200
	.asciz	"main"                          ## string offset=214
	.section	__DWARF,__debug_str_offs,regular,debug
	.long	0
	.long	47
	.long	52
	.long	104
	.long	115
	.long	165
	.long	170
	.long	174
	.long	178
	.long	198
	.long	200
	.long	214
	.section	__DWARF,__debug_addr,regular,debug
Lsection_info0:
.set Lset7, Ldebug_addr_end0-Ldebug_addr_start0 ## Length of contribution
	.long	Lset7
Ldebug_addr_start0:
	.short	5                               ## DWARF version number
	.byte	4                               ## Address size
	.byte	0                               ## Segment selector size
Laddr_table_base0:
	.long	_pMas
	.long	_Mas
	.long	_i
	.long	Lfunc_begin0
Ldebug_addr_end0:
	.section	__DWARF,__debug_names,regular,debug
Ldebug_names_begin:
.set Lset8, Lnames_end0-Lnames_start0   ## Header: unit length
	.long	Lset8
Lnames_start0:
	.short	5                               ## Header: version
	.short	0                               ## Header: padding
	.long	1                               ## Header: compilation unit count
	.long	0                               ## Header: local type unit count
	.long	0                               ## Header: foreign type unit count
	.long	7                               ## Header: bucket count
	.long	7                               ## Header: name count
.set Lset9, Lnames_abbrev_end0-Lnames_abbrev_start0 ## Header: abbreviation table size
	.long	Lset9
	.long	8                               ## Header: augmentation string size
	.ascii	"LLVM0700"                      ## Header: augmentation string
.set Lset10, Lcu_begin0-Lsection_info   ## Compilation unit 0
	.long	Lset10
	.long	0                               ## Bucket 0
	.long	0                               ## Bucket 1
	.long	1                               ## Bucket 2
	.long	4                               ## Bucket 3
	.long	6                               ## Bucket 4
	.long	0                               ## Bucket 5
	.long	7                               ## Bucket 6
	.long	193499014                       ## Hash in Bucket 2
	.long	217009403                       ## Hash in Bucket 2
	.long	-103762318                      ## Hash in Bucket 2
	.long	193495088                       ## Hash in Bucket 3
	.long	2090499946                      ## Hash in Bucket 3
	.long	177678                          ## Hash in Bucket 4
	.long	2090620566                      ## Hash in Bucket 6
	.long	174                             ## String in Bucket 2: Mas
	.long	178                             ## String in Bucket 2: __ARRAY_SIZE_TYPE__
	.long	200                             ## String in Bucket 2: unsigned long
	.long	170                             ## String in Bucket 3: int
	.long	214                             ## String in Bucket 3: main
	.long	198                             ## String in Bucket 4: i
	.long	165                             ## String in Bucket 6: pMas
.set Lset11, Lnames3-Lnames_entries0    ## Offset in Bucket 2
	.long	Lset11
.set Lset12, Lnames2-Lnames_entries0    ## Offset in Bucket 2
	.long	Lset12
.set Lset13, Lnames4-Lnames_entries0    ## Offset in Bucket 2
	.long	Lset13
.set Lset14, Lnames0-Lnames_entries0    ## Offset in Bucket 3
	.long	Lset14
.set Lset15, Lnames6-Lnames_entries0    ## Offset in Bucket 3
	.long	Lset15
.set Lset16, Lnames5-Lnames_entries0    ## Offset in Bucket 4
	.long	Lset16
.set Lset17, Lnames1-Lnames_entries0    ## Offset in Bucket 6
	.long	Lset17
Lnames_abbrev_start0:
	.byte	1                               ## Abbrev code
	.byte	52                              ## DW_TAG_variable
	.byte	3                               ## DW_IDX_die_offset
	.byte	19                              ## DW_FORM_ref4
	.byte	4                               ## DW_IDX_parent
	.byte	25                              ## DW_FORM_flag_present
	.byte	0                               ## End of abbrev
	.byte	0                               ## End of abbrev
	.byte	2                               ## Abbrev code
	.byte	36                              ## DW_TAG_base_type
	.byte	3                               ## DW_IDX_die_offset
	.byte	19                              ## DW_FORM_ref4
	.byte	4                               ## DW_IDX_parent
	.byte	25                              ## DW_FORM_flag_present
	.byte	0                               ## End of abbrev
	.byte	0                               ## End of abbrev
	.byte	3                               ## Abbrev code
	.byte	46                              ## DW_TAG_subprogram
	.byte	3                               ## DW_IDX_die_offset
	.byte	19                              ## DW_FORM_ref4
	.byte	4                               ## DW_IDX_parent
	.byte	25                              ## DW_FORM_flag_present
	.byte	0                               ## End of abbrev
	.byte	0                               ## End of abbrev
	.byte	0                               ## End of abbrev list
Lnames_abbrev_end0:
Lnames_entries0:
Lnames3:
L0:
	.byte	1                               ## Abbreviation code
	.long	57                              ## DW_IDX_die_offset
	.byte	0                               ## DW_IDX_parent
                                        ## End of list: Mas
Lnames2:
L5:
	.byte	2                               ## Abbreviation code
	.long	80                              ## DW_IDX_die_offset
	.byte	0                               ## DW_IDX_parent
                                        ## End of list: __ARRAY_SIZE_TYPE__
Lnames4:
L4:
	.byte	2                               ## Abbreviation code
	.long	95                              ## DW_IDX_die_offset
	.byte	0                               ## DW_IDX_parent
                                        ## End of list: unsigned long
Lnames0:
L1:
	.byte	2                               ## Abbreviation code
	.long	53                              ## DW_IDX_die_offset
	.byte	0                               ## DW_IDX_parent
                                        ## End of list: int
Lnames6:
L3:
	.byte	3                               ## Abbreviation code
	.long	99                              ## DW_IDX_die_offset
	.byte	0                               ## DW_IDX_parent
                                        ## End of list: main
Lnames5:
L2:
	.byte	1                               ## Abbreviation code
	.long	84                              ## DW_IDX_die_offset
	.byte	0                               ## DW_IDX_parent
                                        ## End of list: i
Lnames1:
L6:
	.byte	1                               ## Abbreviation code
	.long	37                              ## DW_IDX_die_offset
	.byte	0                               ## DW_IDX_parent
                                        ## End of list: pMas
	.p2align	2, 0x0
Lnames_end0:
.subsections_via_symbols
	.section	__DWARF,__debug_line,regular,debug
Lsection_line:
Lline_table_start0:
