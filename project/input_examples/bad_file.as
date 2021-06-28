
.entryy	Label1 			  

Label1:	cmp	#-00000123,	NotExistLabel

	dec	#123 	 

	lea 	ExternLabel,	r4	 	

LabelStr:	.string "String-at-label3"   extra-text

WarningLabel: .entry	LabelData  	

LabelData:	.data 	1, -000000000000000000000012345678 ,3	 

Label2:	mov	LabelData, r2

.extern	ExternLabel

	inc	NotExistLabel

	jmp		Label1(r1,r2) 

.entry	NotExistLabel	 

