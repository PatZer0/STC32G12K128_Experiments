import os
import re

def replace_types_in_file(file_path):
    # 定义替换规则
    replacements = {
        r'\b(BYTE|u8|uint8_t)\b': 'unsigned char',
        r'\b(WORD|u16|uint16_t)\b': 'unsigned int',
        r'\b(DWORD|u32|uint32_t)\b': 'unsigned long'
    }

    # 尝试使用不同的编码读取和写入文件
    encodings = ['utf-8', 'gbk']
    for encoding in encodings:
        try:
            # 读取原始文件内容
            with open(file_path, 'r', encoding=encoding) as file:
                content = file.read()

            # 创建备份文件
            backup_file_path = file_path + '.bak'
            with open(backup_file_path, 'w', encoding=encoding) as backup_file:
                backup_file.write(content)

            # 进行替换并计算替换次数
            count = 0
            for pattern, replacement in replacements.items():
                content, num_replacements = re.subn(pattern, replacement, content)
                count += num_replacements

            # 保存修改后的文件
            with open(file_path, 'w', encoding=encoding) as file:
                file.write(content)

            return count
        except UnicodeDecodeError:
            # 如果读取时遇到编码错误，则尝试使用下一个编码
            continue
        except UnicodeEncodeError:
            # 如果写入时遇到编码错误，则尝试使用下一个编码
            continue
    return 0

def main():
    # 遍历当前目录下的 .c 和 .h 文件
    for file in os.listdir('.'):
        if file.endswith('.c') or file.endswith('.h'):
            count = replace_types_in_file(file)
            print(f'在 {file} 中替换了 {count} 处')

if __name__ == '__main__':
    main()
