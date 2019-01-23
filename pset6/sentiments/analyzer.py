import nltk

# initialize the dictionaries as global variables
neg_dict = {}
pos_dict = {}

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        # load negative words into a dict
        i = 0
        f = open(negatives, "r")
        for line in f:
            if line.startswith(";") == False and line.startswith("\n") == False:
                line = line.strip()
                neg_dict[i] = line
                i += 1
        f.close()
        
        # load positive words into a dict
        i = 0
        f = open(positives, "r")
        for line in f:
            if line.startswith(";") == False and line.startswith("\n") == False:
                line = line.strip()
                pos_dict[i] = line
                i += 1
        f.close()

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        # variable to hold the sentiment score
        score = 0
        
        # initialize tokenizer
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        
        for word in tokens:
            if word in neg_dict.values():
                score -= 1
            elif word in pos_dict.values():
                score += 1
            else:
                score += 0
                
        return score