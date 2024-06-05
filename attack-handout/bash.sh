#!/bin/bash

names=("Candle" "Sparkler" "Firecracker" "Dynamite1" "Dynamite2" "Dynamite3")

for name in "${names[@]}"
do
	echo "${name} start-----------------------------------------------"
	txt_file="./ans/${name}.txt"
	if [ -f "$txt_file" ];
	then
		fold -w 12 "$txt_file"
	fi

	code_asm_file="./ans/${name}-code-asm.txt"
	if [ -f "$code_asm_file" ];
	then 
		cat "$code_asm_file"
	fi

	raw_file="./ans/${name}-raw.txt"
	if [ -f "$raw_file" ];
	then
		./bufbomb -t SA23225194 < "$raw_file"
	fi

	echo
done
