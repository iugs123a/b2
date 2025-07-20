# indexer.py
import jieba
import jieba.analyse
from collections import defaultdict
import chardet
import math
import os

class InvertedIndex:
    def __init__(self):
        self.index = defaultdict(lambda: defaultdict(list))  # word -> {filename: [positions]}
        self.doc_freq = defaultdict(int)  # word -> document frequency
        self.doc_word_count = defaultdict(int)  # filename -> total word count
        self.doc_keywords = defaultdict(list)  # filename -> keywords
        self.total_docs = 0
    
    def add_document(self, filepath, words):
        filename = os.path.basename(filepath)
        self.total_docs += 1
        self.doc_word_count[filename] = len(words)
        
        # 提取关键词用于权重计算
        content = ''.join(words)
        keywords = jieba.analyse.extract_tags(content, topK=10, withWeight=True)
        self.doc_keywords[filename] = keywords
        
        # 建立倒排索引
        word_in_doc = set()
        for pos, word in enumerate(words):
            if word.strip() and len(word) > 1:  # 过滤单字符和空白
                self.index[word][filename].append(pos)
                word_in_doc.add(word)
        
        # 更新文档频率
        for word in word_in_doc:
            self.doc_freq[word] += 1
    
    def get_tf_idf(self, word, filename):
        """计算TF-IDF值"""
        if word not in self.index or filename not in self.index[word]:
            return 0
        
        # TF: 词频 / 文档总词数
        tf = len(self.index[word][filename]) / self.doc_word_count[filename]
        
        # IDF: log(总文档数 / 包含该词的文档数)
        idf = math.log(self.total_docs / self.doc_freq[word])
        
        return tf * idf

def read_file_with_detected_encoding(filepath):
    """自动检测文件编码并读取"""
    try:
        # 首先尝试常见编码
        encodings = ['utf-8', 'gbk', 'gb2312', 'utf-16', 'ascii', 'latin1']
        
        for encoding in encodings:
            try:
                with open(filepath, 'r', encoding=encoding) as f:
                    content = f.read()
                    if content:  # 确保读取到内容
                        print(f"  ✅ 使用编码 {encoding} 成功读取")
                        return content
            except UnicodeDecodeError:
                continue
            except Exception:
                continue
        
        # 如果常见编码都失败，使用chardet检测
        with open(filepath, 'rb') as f:
            raw_data = f.read()
            result = chardet.detect(raw_data)
            detected_encoding = result['encoding']
            confidence = result['confidence']
            
            print(f"  🔍 检测到编码: {detected_encoding} (置信度: {confidence:.2f})")
            
            if detected_encoding and confidence > 0.7:
                try:
                    content = raw_data.decode(detected_encoding, errors='ignore')
                    if content:
                        return content
                except Exception:
                    pass
        
        # 最后尝试用utf-8强制读取，忽略错误
        print(f"  ⚠️ 使用UTF-8强制读取（忽略错误）")
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            return f.read()
            
    except Exception as e:
        print(f"❌ 读取文件失败: {filepath} - {e}")
        return None

def build_inverted_index(file_list, debug_mode=False):
    """构建倒排索引"""
    inverted_index = InvertedIndex()
    
    for filepath in file_list:
        filename = os.path.basename(filepath)
        print(f"📖 正在处理: {filename}")
        
        content = read_file_with_detected_encoding(filepath)
        if not content:
            continue
            
        # 分词
        words = list(jieba.cut(content))
        
        if debug_mode:
            print(f"  📊 分词结果: {len(words)} 个词")
            print(f"  🔤 前10个词: {words[:10]}")
        
        inverted_index.add_document(filepath, words)
    
    return inverted_index