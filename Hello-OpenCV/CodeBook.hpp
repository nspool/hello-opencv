#include<vector>

class CodeBook : public vector<CodeElement> {
    
public:
    int t;
    CodeBook() { t=0; } // Default is an empty book
    CodeBook( int n ) : vector<CodeElement>(n) { t=0; } // Construct book of size n
    
};
