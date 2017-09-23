#include<vector>

class CodeElement {
    
public:
    uchar learnHigh[CHANNELS];
    uchar learnLow[CHANNELS];
    uchar max[CHANNELS];
    uchar min[CHANNELS];
    int t_last_update;
    int stale;
    
    CodeElement() {
        for(i = 0; i< CHANNELS; i++) {
            learnHigh[i] = learnLow[i] = max[i] = min[i] = 0;
        }
        t_last_update = stale = 0;
    }
    
    CodeElement& operator=( const CodeElement& ce ) {
        for( i=0; i<CHANNELS; i++ ) {
            learnHigh[i] = ce.learnHigh[i];
            learnLow[i] = ce.learnLow[i];
            min[i] = ce.min[i];
            max[i] = ce.max[i];
        }
        t_last_update = ce.t_last_update;
        stale = ce.stale;
        return *this;
    }
    
    CodeElement( const CodeElement& ce ) { *this = ce; }
    
};

