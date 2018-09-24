#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctype.h>
#include <vector> 
#include <cstring>
#include <ctime>
using namespace std;

/* Word class that stores a word and then each of it's following words */

class Word{
    public:
        string word;
        vector<string> followingWord;
        
        Word(string wrd, string following_wrd){
            word = wrd;
            followingWord.push_back(following_wrd);
        }
};

vector<Word> allWords;     // Vector containing the words inputted. Holds every word found and the words seen following that particular word.
string tweet = "";          // Contains the words of the current tweet being generated.

// Returns the index of where a particular base word in word vector
int getIndexOfAllWords(string word){
    for(int i = 0; i < allWords.size(); i++){
        if(allWords.at(i).word.compare(word) == 0){
            return i;
        }
    }
    return -1;
}

void addWordPair(string word, string followingWord){
    int index = getIndexOfAllWords(word);
    
    if(index == -1){
        allWords.push_back(Word(word, followingWord));
    }
    else{
        allWords.at(index).followingWord.push_back(followingWord);
    }
}


void parseWord(string line){
    size_t wordStartIndex = 0;
    int wordEndIndex;
    
    int followerStartIndex;
    int followerEndIndex;
    
    int wordLength;
    int followingWordLength;
    
    string word = "";
    string follower = "";
    
    // Goes through all the words in the text
    while(wordStartIndex != string::npos){
        wordEndIndex = line.find(" ", wordStartIndex);
        wordLength = wordEndIndex - wordStartIndex;
        
        // Find the next base word
        while(wordLength == 0){
            wordStartIndex++;
            wordEndIndex = line.find(" ", wordStartIndex);
            wordLength = wordEndIndex - wordStartIndex;
            
            // Last word in the text has been reached
            if(wordStartIndex >= line.length()){
                wordStartIndex = line.length();  
                break;
            }
        }
        
        word = line.substr(wordStartIndex, wordLength); // Now holds a single word from the text.
        
        // Now find the next, following word (if there is one)
        if(wordEndIndex != string::npos){
            followerStartIndex = wordEndIndex + 1;
            followerEndIndex = line.find(" ", followerStartIndex);
            followingWordLength = followerEndIndex - followerStartIndex;
            
            while(followingWordLength == 0){
                followerStartIndex++;
                followerEndIndex = line.find(" ", followerStartIndex);
                followingWordLength = followerEndIndex - followerStartIndex;
                if(followerStartIndex >= line.length()){
                    followerStartIndex = line.length(); 
                    break;
                }
            }
            
            word = line.substr(wordStartIndex, wordLength);
            follower = line.substr(followerStartIndex, followingWordLength); // Now contains the word right after the base word
            
            wordStartIndex = followerStartIndex;    // The next base word will be the current base word's follower
        }
        else{
            follower = "";  // If the base word is that last word in the tweet, then it does not have a following word
            wordStartIndex = string::npos;
        }
        
        addWordPair(word, follower);   
                                      
    }
}



int parseTextFile(string filename){
    string currentWord;
    ifstream myfile(filename.c_str());
    
    if (myfile.is_open()) {
        while(getline (myfile,currentWord)){
            parseWord(currentWord);
        }
        myfile.close();
    }
    
    else cout << "Cannot open file."; 
}

void generateTweet(){
    
    
    int wordIndex = (rand() %  allWords.size()); // Randomly select the first word of the tweet
    int followerIndex = 0;
    int numFollowers = 0;
    
    bool tweetIsDone = false;
    string lastWord = ""; // most recent word added, used to search vector
    
    tweet = "";
    tweet = allWords.at(wordIndex).word; // First word of the tweet
    lastWord = tweet;  // Since the tweet is only 1 word long right now, first word = last word
    
    while(!tweetIsDone){
        wordIndex = getIndexOfAllWords(lastWord);
        Word temp = allWords.at(wordIndex);
        
        numFollowers = temp.followingWord.size();
        followerIndex = (rand() % numFollowers);  // Randomly select the next word. Because of word distribution in sentences,
                                                        // the follower word, on average, should be the most frequent word seen after a given base word
        lastWord = temp.followingWord.at(followerIndex);
        
        if(lastWord.length() == 0){    // If the last "word" is an empty string, then the tweet is complete.
            tweetIsDone = true;
        }
        else{
            if(tweet.length() + lastWord.length() > 279){ // Don't make a tweet longer than 279 characters
                tweetIsDone = true;
            }
            else{
                tweet = tweet + " " + lastWord;    // Add the next word to the tweet with a space between the words.
            }
        }
    }
    cout << tweet << endl;
    }



int main(int argc, char* argv[]){
    srand(time(NULL));
    string filename;
    bool userExit = false;
    int userSelection = 0;
    
    if(argc <= 1){
        cout << "Please provide path/to/file.txt" << endl;
        cin  >> filename;
        
    }
    else {
        filename = argv[1]; 
    }
    parseTextFile(filename);
    cout << "Finished parsing... " << endl;
    
    while (!userExit) {
        for(int i = 0; i < 10; i++){
            generateTweet();
            cout << endl;
        }
        cout << "Would you like more tweets?" << endl;
        cout << "1 for YES. 2 for NO." << endl;
        cin >> userSelection;
        
        if (userSelection == 1) {
            continue;
        }
        if (userSelection == 2) {
            userExit = true;
        }
        else if (userSelection != 1) {
            cout << "Invalid choice -- Exiting..." << endl;
            userExit = true;
        }
    }
    
    return 0;
}
