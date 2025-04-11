#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_set>
#include <vector>
#include <cctype>

using namespace std;

// remove punctuation from the word
string sanitize_word(const string& word) {
    string clean;
    for (char c : word) {
        if (isalpha(c) || c == '\'') {
            clean += tolower(c);
        }
    }

    // remove possessive 's at the end (e.g. cat's -> cat)
    if (clean.size() > 2 && clean.substr(clean.size() - 2) == "'s") {
        clean = clean.substr(0, clean.size() - 2);
    }

    return clean;
}

// check if a word is a stopword that should be ignored
bool is_common_word(const string& word) {
    // arbitrary list of stop words - modify as needed
    // uses an unordedred set since checking if an element is in it is O(1)
    static unordered_set<string> common_words = {
        "a", "an", "the", "and", "to", "for",
        "they", "he", "she", "i", "him", "his", "her", "hers", "their",
        "in", "of", "on", "at",  "with", "by"
    };
    return common_words.find(word) != common_words.end();
}

// builds the index using an ordered map of strings to vectors of indices
map<string, vector<int>> build_inverted_index(const string& text) {
    map<string, vector<int>> index;
    istringstream iss(text);
    string word;
    int position = 0;

    while (iss >> word) {
        string clean_word = sanitize_word(word);
        if (!clean_word.empty() && !is_common_word(clean_word)) {
            index[clean_word].push_back(position);
        }
        position++;
    }
    return index;
}

// writes the index from a map to a file
void write_index_to_file(const map<string, vector<int>>& index, const string& output_path) {
    ofstream out(output_path);

    out << "{\n";  // start with curly brace and newline
    bool first = true;
    for (const auto& pair : index) {
        if (!first) out << ",\n";  // end every word-index pair with comma and newline except for last one
        first = false;

        out << "  \"" << pair.first << "\": [";  // word enclosed in double quotes, column, opening bracket
        for (size_t i = 0; i < pair.second.size(); ++i) {
            out << pair.second[i];  // every index of the word
            if (i < pair.second.size() - 1) out << ", ";  // comma unless it's the last index
        }
        out << "]";  // closing bracket
    }
    out << "\n}\n";  // curly braces surrounded by newlines
}

// read input file as a string
string read_file(const string& filepath) {
    ifstream file(filepath);  // assume file opened successfully
    ostringstream ss;
    ss << file.rdbuf();  // convert file to stringstream
    return ss.str();  // convert to string
}

int main() {
    string input_path = "ex_input.txt";
    string output_path = "ex_output.txt";

    string content = read_file(input_path);
    auto index = build_inverted_index(content);
    write_index_to_file(index, output_path);

    cout << "Inverted index written to " << output_path << "\n";
}
