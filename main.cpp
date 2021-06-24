#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <vector>

class TrieNode
{
public:
	std::unordered_map<char, std::unique_ptr<TrieNode>> children;
};

class SuffixTrie
{
public:
	std::unique_ptr<TrieNode> root_;
  	char endSymbol_;

  	SuffixTrie(std::string str) noexcept
	{
    		root_ = std::make_unique<TrieNode>();
    		endSymbol_ = '*';
    		populate_suffix_trie(str);
  	}

	void insert_substring(int i, std::string & str) noexcept
	{
		TrieNode * current_node = root_.get();
		for (int j = 0; j <= i; j++)
		{
			char c = str[j];
			if (current_node->children.find(c) == current_node->children.end())
			{
				std::unique_ptr<TrieNode> new_node = std::make_unique<TrieNode>();
				current_node->children.insert(std::make_pair(c, std::move(new_node)));
			}
			current_node = current_node->children[c].get();
		}
		current_node->children.insert(std::make_pair(endSymbol_, nullptr));
  	}

	void populate_suffix_trie(std::string str) noexcept
	{
		for (int i = 0; i < str.size(); i++)
		{
			insert_substring(i, str);
		}
	}
	
	bool contains(std::string str) const noexcept 
	{
		TrieNode * current_node = root_.get();
		for (char c : str)
		{
			if (current_node->children.find(c) == current_node->children.end())
			{
				return false;
			}
			current_node = current_node->children[c].get();	
		}
		return current_node->children.find(endSymbol_) != current_node->children.end();
  	}
};



class BrowserSearchBar
{
public:
	BrowserSearchBar()
	{
		webpage_dictionary_ = {{"banana",{"webpage1.com", "webpage2.com"}},
				       {"robot",{"webpage1.com", "webpage3.com"}}};
		for (auto pair : webpage_dictionary_)
		{
			std::string word = pair.first;
			std::unique_ptr<SuffixTrie> st = std::make_unique<SuffixTrie>(word);
			word_suffixes_.push_back(std::make_pair(word, std::move(st)));
		}
	}

	void read_from_stream(std::istream& is)
	{
		std::string line;
        	while (std::getline(is, line, '\n'))
        	{
                	std::istringstream is_word(line);
                	std::string word = "";
                	while(std::getline(is_word, word, ' '))
                	{
                        	std::cout << "Word entered: " << word << "\n";
				for (int i = 0; i < word_suffixes_.size(); i++)
				{
					auto word_suffix_trie = word_suffixes_[i].second.get();
					if (word_suffix_trie->contains(word))
					{
						for (auto page : webpage_dictionary_[word_suffixes_[i].first])
						{
							std::cout << "page matched " << page << "\n";
						}
					}

				}
                	}
                	if (word.empty())
                	{
                        	break;
                	}
        	}
        	std::cout << "exiting\n";
	}
private:
	std::unordered_map<std::string, std::unordered_set<std::string>> webpage_dictionary_;
	std::vector<std::pair<std::string, std::unique_ptr<SuffixTrie>>> word_suffixes_; 
};

int main()
{
	std::cout << "this program is used for testing\nPlease enter data stream, exit if no data is given\n";
	auto br = BrowserSearchBar();
	br.read_from_stream(std::cin);
}
