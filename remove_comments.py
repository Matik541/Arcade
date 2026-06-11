#!/usr/bin/env python3
import os
import re

def remove_comments(content):
    """Remove C++ style and C style comments from code"""
    
    lines = content.split('\n')
    result = []
    in_multiline_comment = False
    
    for line in lines:
        if in_multiline_comment:
            if '*/' in line:
                line = line[line.find('*/')+2:]
                in_multiline_comment = False
            else:
                continue
        
        while '/*' in line and '*/' in line:
            start = line.find('/*')
            end = line.find('*/', start)
            line = line[:start] + line[end+2:]
        
        if '/*' in line:
            start = line.find('/*')
            line = line[:start]
            in_multiline_comment = True
        
        if '//' in line and '/*' not in line:
            start = line.find('//')
            line = line[:start]
        
        line = line.rstrip()
        if line or result:
            result.append(line)
    
    while result and not result[-1]:
        result.pop()
    
    return '\n'.join(result) + '\n'

def process_files():
    for root, dirs, files in os.walk('.'):
        if 'build' in root or '.git' in root:
            continue
        
        for file in files:
            if file.endswith(('.cpp', '.h')):
                filepath = os.path.join(root, file)
                print(f"Processing {filepath}...")
                
                with open(filepath, 'r', encoding='utf-8') as f:
                    content = f.read()
                
                new_content = remove_comments(content)
                
                with open(filepath, 'w', encoding='utf-8') as f:
                    f.write(new_content)
                
                print(f"  ✓ Comments removed")

if __name__ == '__main__':
    process_files()
    print("\nAll files processed!")
