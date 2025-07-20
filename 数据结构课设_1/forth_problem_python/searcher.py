# searcher.py
import math
from collections import defaultdict
import jieba
##使用了 TF-IDF 演算法，根据相关性系数进行排序，系数越高，排序越靠前
class SearchEngine:
    def __init__(self, inverted_index):
        self.index = inverted_index
    
    def search(self, query):
        """搜索关键词"""
        # 对查询进行分词
        query_words = [word.strip() for word in jieba.cut(query) if word.strip() and len(word) > 1]
        
        if not query_words:
            return []
        
        # 获取包含查询词的文档
        doc_scores = defaultdict(float)
        doc_positions = defaultdict(list)
        
        for word in query_words:
            if word in self.index.index:
                for filename, positions in self.index.index[word].items():
                    # 使用TF-IDF计算相关性分数
                    tfidf_score = self.index.get_tf_idf(word, filename)
                    doc_scores[filename] += tfidf_score
                    doc_positions[filename].extend(positions)
        
        # 计算额外的排序因子
        final_scores = {}
        for filename, base_score in doc_scores.items():
            # 关键词权重加成
            keyword_bonus = self._get_keyword_bonus(filename, query_words)
            
            # 文档长度归一化
            doc_length_factor = math.log(self.index.doc_word_count[filename] + 1)
            
            # 综合评分
            final_scores[filename] = base_score + keyword_bonus / doc_length_factor
        
        # 按分数排序
        ranked_results = sorted(final_scores.items(), key=lambda x: x[1], reverse=True)
        
        # 返回结果格式：[(filename, positions, score), ...]
        results = []
        for filename, score in ranked_results:
            positions = sorted(set(doc_positions[filename]))  # 去重并排序
            results.append((filename, positions, score))
        
        return results
    
    def _get_keyword_bonus(self, filename, query_words):
        """根据文档关键词给予额外分数"""
        bonus = 0
        if filename in self.index.doc_keywords:
            doc_keywords = dict(self.index.doc_keywords[filename])
            for word in query_words:
                if word in doc_keywords:
                    bonus += doc_keywords[word] * 2  # 关键词匹配给予2倍加成
        return bonus
