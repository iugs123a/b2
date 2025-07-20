# utils.py
import os

def get_all_text_files(folder):
    """获取文件夹下所有文本文件"""
    files = []
    if not os.path.exists(folder):
        return files
        
    for filename in os.listdir(folder):
        if filename.endswith(('.txt', '.md', '.log')):
            files.append(os.path.join(folder, filename))
    return files

# crawler.py
import requests
from bs4 import BeautifulSoup
import time
import os
import random
from urllib.parse import quote

class WeiboCrawler:
    def __init__(self):
        self.headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36',
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
            'Accept-Language': 'zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3',
            'Accept-Encoding': 'gzip, deflate',
            'Connection': 'keep-alive',
        }
        self.session = requests.Session()
        self.session.headers.update(self.headers)
    
    def crawl_weibo(self, keyword, pages=3, save_folder="./data"):
        """爬取微博数据（简化版本，实际使用可能需要处理反爬虫机制）"""
        print(f"🕷️ 开始爬取关键词: {keyword}")
        
        # 这里使用一个简化的示例，实际项目中可能需要更复杂的反爬虫处理
        # 由于微博的反爬虫机制较强，这里提供一个简化的新闻网站爬虫示例
        
        news_sites = [
            f"https://news.baidu.com/ns?word={quote(keyword)}&tn=news",
            f"https://www.toutiao.com/search/?keyword={quote(keyword)}",
        ]
        
        all_content = []
        
        # 模拟爬取新闻内容（实际需要根据具体网站调整）
        for page in range(pages):
            try:
                # 这里可以添加实际的爬虫逻辑
                # 为了演示，我们创建一些示例数据
                sample_content = self._generate_sample_content(keyword, page)
                all_content.append(sample_content)
                
                print(f"✅ 爬取第 {page + 1} 页完成")
                time.sleep(random.uniform(1, 3))  # 随机延迟避免被封
                
            except Exception as e:
                print(f"❌ 爬取第 {page + 1} 页失败: {e}")
                continue
        
        # 保存爬取的内容
        if all_content:
            filename = f"crawled_{keyword}_{int(time.time())}.txt"
            filepath = os.path.join(save_folder, filename)
            
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(f"# 关键词: {keyword} 的爬取结果\n\n")
                f.write("\n\n".join(all_content))
            
            print(f"📁 数据已保存到: {filepath}")
        
        return len(all_content)
    
    def _generate_sample_content(self, keyword, page):
        """生成示例内容（实际项目中替换为真实爬虫逻辑）"""
        samples = [
            f"这是关于{keyword}的第{page+1}页内容。{keyword}在当今社会中扮演着重要角色，我们需要深入了解{keyword}的各个方面。",
            f"根据最新报道，{keyword}相关的研究取得了重大突破。专家认为{keyword}将在未来发挥更大作用。",
            f"分析显示，{keyword}的发展趋势令人关注。业内人士对{keyword}的前景持乐观态度。",
            f"关于{keyword}的讨论越来越热烈，社会各界对{keyword}都给予了高度关注。",
            f"最新数据表明，{keyword}相关指标呈现积极变化，{keyword}的影响力不断扩大。"
        ]
        
        return random.choice(samples) * 3  # 增加内容长度
