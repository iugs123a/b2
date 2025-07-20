# display.py
import jieba
import os
import chardet

def read_file_with_encoding(filepath):
    """智能读取文件，自动检测编码"""
    try:
        # 首先尝试常见编码
        encodings = ['utf-8', 'gbk', 'gb2312', 'utf-16', 'ascii']
        
        for encoding in encodings:
            try:
                with open(filepath, 'r', encoding=encoding) as f:
                    return f.read(), encoding
            except UnicodeDecodeError:
                continue
        
        # 如果常见编码都失败，使用chardet检测
        with open(filepath, 'rb') as f:
            raw_data = f.read()
            result = chardet.detect(raw_data)
            detected_encoding = result['encoding']
            
            if detected_encoding:
                try:
                    content = raw_data.decode(detected_encoding, errors='ignore')
                    return content, detected_encoding
                except Exception:
                    pass
        
        # 最后尝试用utf-8强制读取，忽略错误
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            return f.read(), 'utf-8 (with errors ignored)'
            
    except Exception as e:
        return None, str(e)

def display_results(ranked_results, keyword, folder, debug_mode=False):
    """显示搜索结果"""
    print(f"\n🔍 搜索关键词: '{keyword}'")
    print(f"📚 找到 {len(ranked_results)} 个相关文档")
    print("="*80)
    
    # 对关键词进行分词，用于匹配
    query_words = [word.strip() for word in jieba.cut(keyword) if word.strip() and len(word) > 1]
    
    if debug_mode:
        print(f"🔤 关键词分词结果: {query_words}")
    
    for i, (filename, positions, score) in enumerate(ranked_results[:10], 1):  # 只显示前10个结果
        print(f"\n📄 [{i}] 文件: {filename}")
        print(f"📊 相关性分数: {score:.4f}")
        print(f"🎯 关键词出现位置: {positions[:10]}{'...' if len(positions) > 10 else ''}")
        
        filepath = os.path.join(folder, filename)
        content, encoding_info = read_file_with_encoding(filepath)
        
        if content is None:
            print(f"  ⚠️ 无法读取文件: {encoding_info}")
            print("-" * 60)
            continue
        
        if debug_mode:
            print(f"  📝 文件编码: {encoding_info}")
            print(f"  📏 文件长度: {len(content)} 字符")
        
        # 对内容进行分词
        words = list(jieba.cut(content))
        
        if debug_mode:
            print(f"  🔤 分词后长度: {len(words)} 词")
        
        # 显示上下文片段
        snippets_shown = 0
        shown_positions = set()  # 记录已显示的位置，避免重复
        
        valid_positions = [pos for pos in positions if pos < len(words)]
        
        if debug_mode:
            print(f"  🎯 有效位置数: {len(valid_positions)}/{len(positions)}")
        
        for pos in sorted(valid_positions):  # 按位置排序
            if snippets_shown >= 3:  # 最多显示3个片段
                break
            
            # 避免显示重复的片段
            if any(abs(pos - shown_pos) < 8 for shown_pos in shown_positions):
                if debug_mode:
                    print(f"    跳过位置 {pos}（与已显示位置重复）")
                continue
            
            shown_positions.add(pos)
            
            # 提取上下文
            context_size = 12  # 前后各12个词
            start = max(0, pos - context_size)
            end = min(len(words), pos + context_size + 1)
            
            # 构建上下文片段
            context_words = words[start:end]
            target_word = words[pos]
            target_pos_in_context = pos - start
            
            if debug_mode:
                print(f"    位置 {pos}: 目标词='{target_word}', 上下文范围=[{start}:{end}]")
            
            # 构建带高亮的片段
            highlighted_context = []
            for j, word in enumerate(context_words):
                if j == target_pos_in_context:
                    # 检查是否是查询词的一部分
                    if any(query_word in word or word in query_word for query_word in query_words):
                        highlighted_context.append(f"【{word}】")
                    else:
                        highlighted_context.append(f"《{word}》")  # 用不同符号标记其他匹配
                else:
                    highlighted_context.append(word)
            
            snippet = ''.join(highlighted_context)
            
            # 添加省略号
            prefix = "..." if start > 0 else ""
            suffix = "..." if end < len(words) else ""
            
            print(f"  💡 {prefix}{snippet}{suffix}")
            snippets_shown += 1
        
        if snippets_shown == 0:
            print(f"  ⚠️ 未找到有效的上下文片段")
            if debug_mode:
                print(f"    文档词数: {len(words)}")
                print(f"    位置列表: {positions[:5]}...")
        
        print("-" * 60)
