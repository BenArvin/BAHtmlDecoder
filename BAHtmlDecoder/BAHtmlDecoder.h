//
//  BAHtmlDecoder.h
//
//
//  Created by benarvin on 2024/4/17.
//

#ifndef BAHtmlDecoder_h
#define BAHtmlDecoder_h

#include <string>

struct BAHtmlEscTrieNode;

class BAHtmlDecoder {
public:
    static BAHtmlDecoder& instance() {
        static BAHtmlDecoder instance_;
        return instance_;
    };
    
    std::string decode(const std::string& text);
    
private:
    BAHtmlDecoder();
    BAHtmlDecoder(const BAHtmlDecoder&) {};
    BAHtmlDecoder& operator = (const BAHtmlDecoder&);
    BAHtmlEscTrieNode* root;
    
    void addPattern(const std::string& pattern, unsigned short decodedVal);
    void prepare();
    void enableNumExp();
};

#endif /* BAHtmlDecoder_h */
