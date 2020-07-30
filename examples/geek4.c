// https://www.geeksforgeeks.org/count-of-words-that-are-present-in-all-the-given-sentences/
// https://www.geeksforgeeks.org/tag/cpp-unordered_map/
/*
Count of words that are present in all the given sentences
Given n sentences. The task is to count the number of words that appear in all of these
sentences. Note that every word consists of only lowercase English alphabets.

Examples:

Input: arr[] = {
    "there is a cow",
    "cow is our mother",
    "cow gives us milk and milk is sweet",
    "there is a boy who loves cow"}
Output: 2
Only the words "is" and "cow" appear in all of the sentences.

Input: arr[] = {
    "abc aac abcd ccc",
    "ac aa abc cca",
    "abca aaac abcd ccc"}
Output: 0

Naive Approach: The naive approach is to take every word of the first sentence and compare it
                with rest of the lines if it is also present in all lines then increment the count.


Efficient Approach: For all the words of the first sentence, we can check if it is also present in
                    all the other sentences in constant time by using a map. Store all the words
                    of the first sentence in a map and check how many of these stored words are
                    present in all of the other sentences.
*/
#ifndef CXX
// C implementation of the approach 

#include <stc/cmap.h>
#include <stc/cvec.h>
#include <stc/cstr.h>

declare_cvec_str();
declare_cmap_str(sb, bool);
declare_cvec(sb, cmap_sb_entry_t, cmap_sb_entry_destroy, c_no_compare);
  
// Function to return the count of common words 
// in all the sentences 
int commonWords(cvec_str S) 
{ 
    int m, n, i, j; 
  
 
    // To store all the words of first string 
    cvec_sb ans = cvec_init; 
  
    // m will store number of strings in given vector 
    m = cvec_size(S);
  
    i = 0; 
  
    // Extract all words of first string and store it in ans 
    while (i < cstr_size(S.data[0])) { 
        // To store separate words 
        cstr_t word = cstr_init; 
        cmap_sb_entry_t tmp = {cstr_init, false};

        while (i < cstr_size(S.data[0]) && S.data[0].str[i] != ' ') { 
            cstr_push_back(&word, S.data[0].str[i]); 
            i++; 
        } 
  
        // Increase i to get at starting index 
        // of the next word 
        i++; 
  
        // If str is not empty store it in map 
        if (!cstr_empty(word)) { 
            tmp.key = cstr_move(&word);
            tmp.value = true;
            cvec_sb_push_back(&ans, tmp);
        }
    } 

    // Start from 2nd line check if any word of 
    // the first string did not match with 
    // some word in the current line 
    for (j = 1; j < m; j++) { 
        // It will be used to check if a word is present 
        // in a particuler string 
        cmap_sb has = cmap_init; 
        i = 0; 
  
        while (i < cstr_size(S.data[j])) { 
            cstr_t word = cstr_init; 
            while (i < cstr_size(S.data[j]) && S.data[j].str[i] != ' ') { 
                cstr_push_back(&word, S.data[j].str[i]); 
                i++; 
            } 
            i++; 
            if (!cstr_empty(word)) { 
                cmap_sb_put(&has, word.str, true);
            }
            cstr_destroy(&word);
        } 
  
        // Check all words of this vector 
        // if it is not present in current line 
        // make it false 
        for (int k = 0; k < cvec_size(ans); k++) { 
            if (ans.data[k].value != false 
                && cmap_sb_insert(&has, ans.data[k].key.str, false)->value == false) { 
                ans.data[k].value = false; 
            } 
  
            // This line is used to consider only distinct words                 
            else if (ans.data[k].value != false
                     && cmap_sb_insert(&has, ans.data[k].key.str, false)->value == true) { 
                cmap_sb_put(&has, ans.data[k].key.str, false); 
            } 
        } 
        cmap_sb_destroy(&has);
    } 
  
    // This function will print 
    // the count of common words 
    int cnt = 0; 
  
    // If current word appears in all the sentences 
    for (int k = 0; k < cvec_size(ans); k++) { 
        if (ans.data[k].value == true) 
            cnt++; 
    }

    cvec_sb_destroy(&ans);
    return cnt; 
} 
  
// Driver code 
int main() 
{ 
    cvec_str S = cvec_init; 
    cvec_str_push_back(&S, cstr_make("there is a cow")); 
    cvec_str_push_back(&S, cstr_make("cow is our mother")); 
    cvec_str_push_back(&S, cstr_make("cow gives us milk and milk is sweet")); 
    cvec_str_push_back(&S, cstr_make("there is a boy who loves cow")); 
  
    printf("%d\n", commonWords(S)); 
    cvec_str_destroy(&S);
    return 0; 
}


#else // ========================= c++ =======================


// C++ implementation of the approach 
#include <bits/stdc++.h> 
using namespace std; 
  
// Function to return the count of common words 
// in all the sentences 
int commonWords(vector<string> S) 
{ 
    int m, n, i, j; 
  
    // To store separate words 
    string word; 
  
    // It will be used to check if a word is present 
    // in a particuler string 
    unordered_map<string, bool> has; 
  
    // To store all the words of first string 
    vector<pair<string, bool> > ans; 
  
    pair<string, bool> tmp; 
  
    // m will store number of strings in given vector 
    m = S.size(); 
  
    i = 0; 
  
    // Extract all words of first string and store it in ans 
    while (i < S[0].size()) { 
        word = ""; 
        while (i < S[0].size() && S[0][i] != ' ') { 
            word += S[0][i]; 
            i++; 
        } 
  
        // Increase i to get at starting index 
        // of the next word 
        i++; 
  
        // If word is not empty store it in map 
        if (word != "") { 
            tmp = make_pair(word, true); 
            ans.push_back(tmp); 
        } 
    } 
  
    // Start from 2nd line check if any word of 
    // the first string did not match with 
    // some word in the current line 
    for (j = 1; j < m; j++) { 
        has.clear(); 
        i = 0; 
  
        while (i < S[j].size()) { 
            word = ""; 
            while (i < S[j].size() && S[j][i] != ' ') { 
                word += S[j][i]; 
                i++; 
            } 
            i++; 
            if (word != "") { 
                has[word] = true; 
            } 
        } 
  
        // Check all words of this vector 
        // if it is not present in current line 
        // make it false 
        for (int k = 0; k < ans.size(); k++) { 
            if (ans[k].second != false
                && has[ans[k].first] == false) { 
                ans[k].second = false; 
            } 
  
            // This line is used to consider only distinct words 
            else if (ans[k].second != false
                     && has[ans[k].first] == true) { 
                has[ans[k].first] = false; 
            } 
        } 
    } 
  
    // This function will print 
    // the count of common words 
    int cnt = 0; 
  
    // If current word appears in all the sentences 
    for (int k = 0; k < ans.size(); k++) { 
        if (ans[k].second == true) 
            cnt++; 
    } 
  
    return cnt; 
} 
  
// Driver code 
int main() 
{ 
    vector<string> S; 
    S.push_back("there is a cow"); 
    S.push_back("cow is our mother"); 
    S.push_back("cow gives us milk and milk is sweet"); 
    S.push_back("there is a boy who loves cow"); 
  
    cout << commonWords(S); 
  
    return 0; 
} 
#endif