# main.py
import os
import sys
from indexer import build_inverted_index
from searcher import SearchEngine
from display import display_results
from utils import get_all_text_files
from crawler import WeiboCrawler

def main():
    data_folder = "./data"
    
    # 确保数据文件夹存在
    if not os.path.exists(data_folder):
        os.makedirs(data_folder)
    
    print("🔍 搜索引擎启动中...")
    print("1. 使用本地文本文件")
    print("2. 爬取微博数据（未实现）")
    print("3. 调试模式（显示详细信息）")
    choice = input("请选择模式 (1/2/3): ")
    
    debug_mode = (choice == "3")
    
    if choice == "2":
        print("🕷️ 启动微博爬虫...")
        crawler = WeiboCrawler()
        keywords = input("请输入要爬取的关键词（多个关键词用空格分隔）: ").split()
        pages = int(input("请输入要爬取的页数（建议1-5页）: "))
        
        for keyword in keywords:
            crawler.crawl_weibo(keyword, pages, data_folder)
    
    print("🔍 正在加载文件和构建倒排索引...")
    text_files = get_all_text_files(data_folder)
    
    if not text_files:
        print("❌ 未找到任何文本文件，请检查data文件夹")
        return
    
    inverted_index = build_inverted_index(text_files, debug_mode)
    search_engine = SearchEngine(inverted_index)
    print(f"✅ 索引建立完成，共索引了 {len(text_files)} 个文件")
    
    # 搜索循环
    while True:
        print("\n" + "="*50)
        keyword = input("请输入关键词（输入 exit 退出）：")
        if keyword.lower() == "exit":
            break
        
        results = search_engine.search(keyword)
        if not results:
            print("❌ 未找到匹配内容。")
            continue
        
        display_results(results, keyword, data_folder, debug_mode)

if __name__ == "__main__":
    main()
