#!/bin/bash

# 可执行文件名称
program="bomb"


#使用objdump反汇编main函数并保存到文件
objdump -d "$program" | awk '/<main>:/,/^$/' > main_asm.txt

#继续保存phase_1 ~ phase_6，以及secret_phase函数到文件
# 函数名称列表
phases=("phase_1" "phase_2" "phase_3" "phase_4" "phase_5" "phase_6")

# 反汇编并保存每个函数的汇编代码
for phase in "${phases[@]}"; do
    output_file="${phase}.txt"
    objdump -d "$program" | awk "/<$phase>:/,/^$/" > "$output_file"
    if [[ -s "$output_file" ]]; then
        echo "Saved ${output_file}"
    else
        echo "Failed to save ${output_file}. Function ${phase} may not exist in ${program}."
        rm "$output_file"
    fi
done

objdump -d "$program" | awk '/<secret_phase>:/,/^$/' > "secret_phase_asm.txt"
