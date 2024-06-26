//
//  BAHtmlDecoder.cpp
//
//
//  Created by benarvin on 2024/4/17.
//

#include "BAHtmlDecoder.h"
#include <vector>
#include <map>
#include <queue>
#include <sstream>

typedef struct {
    std::string escKey;
    unsigned short uchar;
} BAHtmlEscMapItem;

static BAHtmlEscMapItem gBAAsciiHTMLEscMap[] = {
    // A.2.2. Special characters
    { "&quot;", 34 },
    { "&amp;", 38 },
    //    { "&apos;", 39 },
    { "&lt;", 60 },
    { "&gt;", 62 },
    
    // A.2.1. Latin-1 characters
    { "&nbsp;", 160 },
    { "&iexcl;", 161 },
    { "&cent;", 162 },
    { "&pound;", 163 },
    { "&curren;", 164 },
    { "&yen;", 165 },
    { "&brvbar;", 166 },
    { "&sect;", 167 },
    { "&uml;", 168 },
    { "&copy;", 169 },
    { "&ordf;", 170 },
    { "&laquo;", 171 },
    { "&not;", 172 },
    { "&shy;", 173 },
    { "&reg;", 174 },
    { "&macr;", 175 },
    { "&deg;", 176 },
    { "&plusmn;", 177 },
    { "&sup2;", 178 },
    { "&sup3;", 179 },
    { "&acute;", 180 },
    { "&micro;", 181 },
    { "&para;", 182 },
    { "&middot;", 183 },
    { "&cedil;", 184 },
    { "&sup1;", 185 },
    { "&ordm;", 186 },
    { "&raquo;", 187 },
    { "&frac14;", 188 },
    { "&frac12;", 189 },
    { "&frac34;", 190 },
    { "&iquest;", 191 },
    { "&Agrave;", 192 },
    { "&Aacute;", 193 },
    { "&Acirc;", 194 },
    { "&Atilde;", 195 },
    { "&Auml;", 196 },
    { "&Aring;", 197 },
    { "&AElig;", 198 },
    { "&Ccedil;", 199 },
    { "&Egrave;", 200 },
    { "&Eacute;", 201 },
    { "&Ecirc;", 202 },
    { "&Euml;", 203 },
    { "&Igrave;", 204 },
    { "&Iacute;", 205 },
    { "&Icirc;", 206 },
    { "&Iuml;", 207 },
    { "&ETH;", 208 },
    { "&Ntilde;", 209 },
    { "&Ograve;", 210 },
    { "&Oacute;", 211 },
    { "&Ocirc;", 212 },
    { "&Otilde;", 213 },
    { "&Ouml;", 214 },
    { "&times;", 215 },
    { "&Oslash;", 216 },
    { "&Ugrave;", 217 },
    { "&Uacute;", 218 },
    { "&Ucirc;", 219 },
    { "&Uuml;", 220 },
    { "&Yacute;", 221 },
    { "&THORN;", 222 },
    { "&szlig;", 223 },
    { "&agrave;", 224 },
    { "&aacute;", 225 },
    { "&acirc;", 226 },
    { "&atilde;", 227 },
    { "&auml;", 228 },
    { "&aring;", 229 },
    { "&aelig;", 230 },
    { "&ccedil;", 231 },
    { "&egrave;", 232 },
    { "&eacute;", 233 },
    { "&ecirc;", 234 },
    { "&euml;", 235 },
    { "&igrave;", 236 },
    { "&iacute;", 237 },
    { "&icirc;", 238 },
    { "&iuml;", 239 },
    { "&eth;", 240 },
    { "&ntilde;", 241 },
    { "&ograve;", 242 },
    { "&oacute;", 243 },
    { "&ocirc;", 244 },
    { "&otilde;", 245 },
    { "&ouml;", 246 },
    { "&divide;", 247 },
    { "&oslash;", 248 },
    { "&ugrave;", 249 },
    { "&uacute;", 250 },
    { "&ucirc;", 251 },
    { "&uuml;", 252 },
    { "&yacute;", 253 },
    { "&thorn;", 254 },
    { "&yuml;", 255 },
    
    // A.2.2. Special characters cont'd
    { "&OElig;", 338 },
    { "&oelig;", 339 },
    { "&Scaron;", 352 },
    { "&scaron;", 353 },
    { "&Yuml;", 376 },
    
    // A.2.3. Symbols
    { "&fnof;", 402 },
    
    // A.2.2. Special characters cont'd
    { "&circ;", 710 },
    { "&tilde;", 732 },
    
    // A.2.3. Symbols cont'd
    { "&Alpha;", 913 },
    { "&Beta;", 914 },
    { "&Gamma;", 915 },
    { "&Delta;", 916 },
    { "&Epsilon;", 917 },
    { "&Zeta;", 918 },
    { "&Eta;", 919 },
    { "&Theta;", 920 },
    { "&Iota;", 921 },
    { "&Kappa;", 922 },
    { "&Lambda;", 923 },
    { "&Mu;", 924 },
    { "&Nu;", 925 },
    { "&Xi;", 926 },
    { "&Omicron;", 927 },
    { "&Pi;", 928 },
    { "&Rho;", 929 },
    { "&Sigma;", 931 },
    { "&Tau;", 932 },
    { "&Upsilon;", 933 },
    { "&Phi;", 934 },
    { "&Chi;", 935 },
    { "&Psi;", 936 },
    { "&Omega;", 937 },
    { "&alpha;", 945 },
    { "&beta;", 946 },
    { "&gamma;", 947 },
    { "&delta;", 948 },
    { "&epsilon;", 949 },
    { "&zeta;", 950 },
    { "&eta;", 951 },
    { "&theta;", 952 },
    { "&iota;", 953 },
    { "&kappa;", 954 },
    { "&lambda;", 955 },
    { "&mu;", 956 },
    { "&nu;", 957 },
    { "&xi;", 958 },
    { "&omicron;", 959 },
    { "&pi;", 960 },
    { "&rho;", 961 },
    { "&sigmaf;", 962 },
    { "&sigma;", 963 },
    { "&tau;", 964 },
    { "&upsilon;", 965 },
    { "&phi;", 966 },
    { "&chi;", 967 },
    { "&psi;", 968 },
    { "&omega;", 969 },
    { "&thetasym;", 977 },
    { "&upsih;", 978 },
    { "&piv;", 982 },
    
    // A.2.2. Special characters cont'd
    { "&ensp;", 8194 },
    { "&emsp;", 8195 },
    { "&thinsp;", 8201 },
    { "&zwnj;", 8204 },
    { "&zwj;", 8205 },
    { "&lrm;", 8206 },
    { "&rlm;", 8207 },
    { "&ndash;", 8211 },
    { "&mdash;", 8212 },
    { "&lsquo;", 8216 },
    { "&rsquo;", 8217 },
    { "&sbquo;", 8218 },
    { "&ldquo;", 8220 },
    { "&rdquo;", 8221 },
    { "&bdquo;", 8222 },
    { "&dagger;", 8224 },
    { "&Dagger;", 8225 },
    // A.2.3. Symbols cont'd
    { "&bull;", 8226 },
    { "&hellip;", 8230 },
    
    // A.2.2. Special characters cont'd
    { "&permil;", 8240 },
    
    // A.2.3. Symbols cont'd
    { "&prime;", 8242 },
    { "&Prime;", 8243 },
    
    // A.2.2. Special characters cont'd
    { "&lsaquo;", 8249 },
    { "&rsaquo;", 8250 },
    
    // A.2.3. Symbols cont'd
    { "&oline;", 8254 },
    { "&frasl;", 8260 },
    
    // A.2.2. Special characters cont'd
    { "&euro;", 8364 },
    
    // A.2.3. Symbols cont'd
    { "&image;", 8465 },
    { "&weierp;", 8472 },
    { "&real;", 8476 },
    { "&trade;", 8482 },
    { "&alefsym;", 8501 },
    { "&larr;", 8592 },
    { "&uarr;", 8593 },
    { "&rarr;", 8594 },
    { "&darr;", 8595 },
    { "&harr;", 8596 },
    { "&crarr;", 8629 },
    { "&lArr;", 8656 },
    { "&uArr;", 8657 },
    { "&rArr;", 8658 },
    { "&dArr;", 8659 },
    { "&hArr;", 8660 },
    { "&forall;", 8704 },
    { "&part;", 8706 },
    { "&exist;", 8707 },
    { "&empty;", 8709 },
    { "&nabla;", 8711 },
    { "&isin;", 8712 },
    { "&notin;", 8713 },
    { "&ni;", 8715 },
    { "&prod;", 8719 },
    { "&sum;", 8721 },
    { "&minus;", 8722 },
    { "&lowast;", 8727 },
    { "&radic;", 8730 },
    { "&prop;", 8733 },
    { "&infin;", 8734 },
    { "&ang;", 8736 },
    { "&and;", 8743 },
    { "&or;", 8744 },
    { "&cap;", 8745 },
    { "&cup;", 8746 },
    { "&int;", 8747 },
    { "&there4;", 8756 },
    { "&sim;", 8764 },
    { "&cong;", 8773 },
    { "&asymp;", 8776 },
    { "&ne;", 8800 },
    { "&equiv;", 8801 },
    { "&le;", 8804 },
    { "&ge;", 8805 },
    { "&sub;", 8834 },
    { "&sup;", 8835 },
    { "&nsub;", 8836 },
    { "&sube;", 8838 },
    { "&supe;", 8839 },
    { "&oplus;", 8853 },
    { "&otimes;", 8855 },
    { "&perp;", 8869 },
    { "&sdot;", 8901 },
    { "&lceil;", 8968 },
    { "&rceil;", 8969 },
    { "&lfloor;", 8970 },
    { "&rfloor;", 8971 },
    { "&lang;", 9001 },
    { "&rang;", 9002 },
    { "&loz;", 9674 },
    { "&spades;", 9824 },
    { "&clubs;", 9827 },
    { "&hearts;", 9829 },
    { "&diams;", 9830 }
};

typedef enum : int {
    BAHtmlEscTrieNodeTypeDefault = 0,
    BAHtmlEscTrieNodeTypeNum = 1,
    BAHtmlEscTrieNodeTypeHexNum = 2,
} BAHtmlEscTrieNodeType;

struct BAHtmlEscTrieNode {
    std::map<char, BAHtmlEscTrieNode*> children;
    BAHtmlEscTrieNode* fail;
    
    BAHtmlEscTrieNodeType type = BAHtmlEscTrieNodeTypeDefault;
    bool isPattern = false;
    unsigned short decodedVal = 0;
    int numNodeStrLen = 0;

    BAHtmlEscTrieNode() : fail(nullptr) {}
};

BAHtmlDecoder::BAHtmlDecoder() {
    this->root = new BAHtmlEscTrieNode();
    for (unsigned i = 0; i < sizeof(gBAAsciiHTMLEscMap) / sizeof(BAHtmlEscMapItem); ++i) {
        this->addPattern(gBAAsciiHTMLEscMap[i].escKey, gBAAsciiHTMLEscMap[i].uchar);
    }
    this->prepare();
    this->enableNumExp();
}

void BAHtmlDecoder::addPattern(const std::string& pattern, unsigned short decodedVal) {
    BAHtmlEscTrieNode* cur = root;
    for (char c : pattern) {
        if (cur->children.find(c) == cur->children.end()) {
            cur->children[c] = new BAHtmlEscTrieNode();
        }
        cur = cur->children[c];
    }
    cur->isPattern = true;
    cur->decodedVal = decodedVal;
}

void BAHtmlDecoder::prepare() {
    std::vector<BAHtmlEscTrieNode*> levelNodes;
    for (auto& pair : root->children) {
        pair.second->fail = root;
        levelNodes.push_back(pair.second);
    }

    for (int i = 0; i < levelNodes.size(); ++i) {
        BAHtmlEscTrieNode* curr = levelNodes[i];

        for (auto& pair : curr->children) {
            char c = pair.first;
            BAHtmlEscTrieNode* child = pair.second;
            BAHtmlEscTrieNode* failNode = curr->fail;

            while (failNode != nullptr && failNode->children.find(c) == failNode->children.end()) {
                failNode = failNode->fail;
            }

            if (failNode == nullptr) {
                child->fail = root;
            } else {
                child->fail = failNode->children[c];
            }

            levelNodes.push_back(child);
        }
    }
}
    
void BAHtmlDecoder::enableNumExp() {
    BAHtmlEscTrieNode* numNode = new BAHtmlEscTrieNode();
    numNode->fail = root;
    numNode->type = BAHtmlEscTrieNodeTypeNum;
    
    BAHtmlEscTrieNode* hexNumNode = new BAHtmlEscTrieNode();
    hexNumNode->fail = root;
    hexNumNode->type = BAHtmlEscTrieNodeTypeHexNum;
    
    BAHtmlEscTrieNode* branchRoot = root->children['&'];
    branchRoot->children['#'] = numNode;
    
    numNode->children['x'] = hexNumNode;
    numNode->children['X'] = hexNumNode;
}
    
std::string BAHtmlDecoder::decode(const std::string& text) {
    if (text.empty()) {
        return text;
    }
    std::stringstream ress;
    BAHtmlEscTrieNode* cur = root;
    unsigned long loc = 0;
    for (unsigned long i=0; i<text.length(); i++) {
        char c = text[i];
        if (cur->type == BAHtmlEscTrieNodeTypeNum) {
            if (cur->numNodeStrLen <= 0) {
                if (c == 'x' || c == 'X') {
                    cur = cur->children[c];
                    cur->numNodeStrLen = 0;
                } else if (c >= '0' && c <= '9') {
                    cur->numNodeStrLen++;
                } else {
                    ress << text.substr(loc, i - loc);
                    cur = cur->fail;
                    loc = i;
                }
            } else {
                if (c == ';') {
                    int valTmp = std::stoi(text.substr(loc + 2, cur->numNodeStrLen));
                    if (valTmp > 0 && valTmp < USHRT_MAX) {
                        ress << static_cast<char>(valTmp % 256);
                        loc = i + 1;
                    } else {
                        ress << text.substr(loc, i - loc);
                        loc = i;
                    }
                    cur = cur->fail;
                } else if (c >= '0' && c <= '9') {
                    cur->numNodeStrLen++;
                } else {
                    ress << text.substr(loc, i - loc);
                    cur = cur->fail;
                    loc = i;
                }
            }
        } else if (cur->type == BAHtmlEscTrieNodeTypeHexNum) {
            if (cur->numNodeStrLen <= 0) {
                if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
                    cur->numNodeStrLen++;
                } else {
                    ress << text.substr(loc, i - loc);
                    cur = cur->fail;
                    loc = i;
                }
            } else {
                if (c == ';') {
                    int valTmp = std::stoi("0x" + text.substr(loc + 3, cur->numNodeStrLen), nullptr, 16);
                    if (valTmp > 0 && valTmp < USHRT_MAX) {
                        ress << static_cast<char>(valTmp % 256);
                        loc = i + 1;
                    } else {
                        ress << text.substr(loc, i - loc);
                        loc = i;
                    }
                    cur = cur->fail;
                } else if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
                    cur->numNodeStrLen++;
                } else {
                    ress << text.substr(loc, i - loc);
                    cur = cur->fail;
                    loc = i;
                }
            }
        } else {
            while (cur != nullptr && cur->children.find(c) == cur->children.end()) {
                cur = cur->fail;
                if (loc < i) {
                    ress << text.substr(loc, i - loc);
                }
                loc = i;
            }
            
            if (cur == nullptr) {
                ress << c;
                cur = root;
                loc = i + 1;
            } else {
                cur = cur->children[c];
                if (cur->isPattern) {
                    if (cur->decodedVal < 32 || cur->decodedVal > 126) {
                        ress << ' ';
                    } else {
                        ress << static_cast<char>(cur->decodedVal);
                    }
                    loc = i + 1;
                }
            }
            if (cur->type == BAHtmlEscTrieNodeTypeNum) {
                cur->numNodeStrLen = 0;
            }
        }
    }
    return ress.str();
}
