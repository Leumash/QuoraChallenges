/*

Ontology

Quora has many questions on different topics, and a common product use-case for our @mention selectors and search service is to look-up questions under a certain topic as quickly as possible.

For this problem, imagine a simplified version of Quora where each question has only one topic associated with it. In turn, the topics form a simplified
ontology where each topic has a list of children, and all topics are descendants of a single root topic.

Design a system that allows for fast searches of questions under topics. There are NN topics, MM questions, and KK queries, given in this order. Each query has a desired topic as well as a desired string prefix. For each query, return the number of questions that fall under the queried topic and begin with the desired string. When considering topics, we want to include all descendants of the queried topic as well as the queried topic itself. In other words, each query searches over the subtree of the topic.

The topic ontology is given in the form of a flattened tree of topic names, where each topic may optionally have children. If a topic has children, they are listed after it within parentheses, and those topics may have children of their own, etc. See the sample for the exact input format. The tree is guaranteed to have a single root topic.

For ease of parsing, each topic name will be composed of English alphabetical characters, and each question and query text will be composed of English alphabetical characters, spaces, and question marks. Each question and query text will be well behaved: there will be no consecutive spaces or leading/trailing spaces. All queries, however, are case sensitive.

Constraints

For 100% of the test data, 1≤N,M,K≤1051≤N,M,K≤105 and the input file is smaller than 5MB
For 50% of the test data, 1≤N,M,K≤2⋅1041≤N,M,K≤2⋅104 and the input file is smaller than 1MB

Input Format

Line 1: One integer NN
Line 2: NN topics arranged in a flat tree (see sample)
Line 3: One integer MM
Line 4...M+3: Each line contains a topic name, followed by a colon and a space, and then the question text.
Line M+4: One integer KK
Line M+5...M+K+4: Each line contains a topic name, followed by a space, and then the query text.

Output Format

Line 1...K: Line ii should contain the answer for the iith query.

Sample Input

6
Animals ( Reptiles Birds ( Eagles Pigeons Crows ) )
5
Reptiles: Why are many reptiles green?
Birds: How do birds fly?
Eagles: How endangered are eagles?
Pigeons: Where in the world are pigeons most densely populated?
Eagles: Where do most eagles live?
4
Eagles How en
Birds Where
Reptiles Why do
Animals Wh


Sample Output

1
2
0
3

*/

/*

Three parts to this problem

1. Topic Input
2. Question Input
3. Query Input

For each of the three steps, after reading in the input, create data structures to hold data and auxiliary data.

1. Create a map such that key: topic -> values: list of ancestors
2. Create a Trie on questions. Use STL unordered_map for the Trie Tree. The keys are all possible valid queries, or, all possible prefixes on questions. The values are another map. The second map is an unordered_map topics to count.
    - For each question
        - For each prefix
            - Enter into the Trie, incremental values into the second map, of the list of ancestors (and self)
3. Query the data structure in step 2. The query is the key to the first map. Then the topic is the key to the second map. Return the integer key for the final answer.

Runtime Analysis

1. Not implemented yet so not sure
2. Worst case is O(n^2). Average case is O(n log n).
    - Runtime is for each question, iterate through each of the ancestors
        - Worst case is a tree where
3. For a single query, runtime is O(1), or O(m) where m is the length of the query.

Space Complexity

1. Not implemented yet so not sure
2. Depends on input. Space required is O(m * n) where m is all possible prefixes of all questions and n is all possible topics.
3. O(1)

*/

#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <cassert>

using namespace std;

string ReadTopicInput()
{
    int N;
    string line;

    cin >> N;

    getline(cin, line);     // Get rid of the extra newline from 'N'
    getline(cin, line);

    return line;
}

vector<string> ReadQuestionInput()
{
    int M;
    string line;
    vector<string> topicAndQuestions;

    cin >> M;

    getline(cin, line);     // Get rid of the extra newline from 'M'

    for (int i=0; i<M; ++i)
    {
        getline(cin, line); 

        topicAndQuestions.push_back(line);
    }

    return topicAndQuestions;
}

vector<string> ReadQueryInput()
{
    int M;
    string line;
    vector<string> topicAndQueries;

    cin >> M;

    getline(cin, line);     // Get rid of the extra newline from 'M'

    for (int i=0; i<M; ++i)
    {
        getline(cin, line); 

        topicAndQueries.push_back(line);
    }

    return topicAndQueries;
}

vector<string> SplitString(const string &line)
{
    vector<string> toReturn;

    string word;
    stringstream ss(line);

    while (ss >> word)
    {
        toReturn.push_back(word);
    }

    return toReturn;
}

bool IsTopic(const string &token)
{
    return token != "(" && token != ")";
}

unordered_map<string,vector<string>> GetTopicsToTopicsOnPaths(const string &line)
{
    vector<string> tokens = SplitString(line);

    unordered_map<string,vector<string>> topicsToTopicsOnPaths;

    vector<string> currentTopicsOnPath;

    for (unsigned int i=0; i<tokens.size(); ++i)
    {
        if (IsTopic(tokens.at(i)))
        {
            currentTopicsOnPath.push_back(tokens.at(i));
            topicsToTopicsOnPaths[tokens.at(i)] = currentTopicsOnPath;
            currentTopicsOnPath.pop_back();
        }
        else if (tokens.at(i) == "(")
        {
            currentTopicsOnPath.push_back(tokens.at(i-1));
        }
        else    // tokens.at(i) == ")"
        {
            currentTopicsOnPath.pop_back();
        }
    }

    return topicsToTopicsOnPaths;
}

pair<string,string> ParseQuestion(const string &line)
{
    int pos = line.find_first_of(" ");

    return make_pair(line.substr(0, pos-1), line.substr(pos + 1));
}

vector<pair<string,string>> GetTopicsToQuestions(const vector<string> &questions)
{
    vector<pair<string,string>> topicsToQuestions;

    for (const string &question : questions)
    {
        topicsToQuestions.push_back(ParseQuestion(question));
    }

    return topicsToQuestions;
}

unordered_map<string,unordered_map<string,int>> GetPrefixToTopicToCount(const vector<pair<string,string>> &topicsToQuestions, const unordered_map<string,vector<string>> &topicsToTopicsOnPaths)
{
    unordered_map<string,unordered_map<string,int>> prefixesToTopicsToCounts;

    for (const auto topicToQuestion : topicsToQuestions)
    {
        for (unsigned int prefixSize=1; prefixSize <=topicToQuestion.second.size(); ++prefixSize)
        {
            string prefix = topicToQuestion.second.substr(0, prefixSize);

            for (const string &topicOnPath : topicsToTopicsOnPaths.at(topicToQuestion.first))
            {
                if (prefixesToTopicsToCounts[prefix].count(topicOnPath) == 0)
                {
                    prefixesToTopicsToCounts[prefix][topicOnPath] = 0;
                }

                ++prefixesToTopicsToCounts[prefix][topicOnPath];
            }
        }
    }

    return prefixesToTopicsToCounts;
}

vector<pair<string,string>> GetTopicsAndQueries(const vector<string> &queries)
{
    vector<pair<string,string>> topicsAndQueries;

    for (const string &query : queries)
    {
        int pos = query.find_first_of(" ");

        topicsAndQueries.push_back(make_pair(query.substr(0, pos), query.substr(pos + 1)));
    }

    return topicsAndQueries;
}

void SolveQueries(const vector<pair<string,string>> &topicsAndQueries, const unordered_map<string,unordered_map<string,int>> &prefixesToTopicsToCounts)
{
    for (const auto topicAndQuery : topicsAndQueries)
    {
        if (prefixesToTopicsToCounts.count(topicAndQuery.second) == 0)
        {
            cout << 0 << endl;
        }
        else if (prefixesToTopicsToCounts.at(topicAndQuery.second).count(topicAndQuery.first) == 0)
        {
            cout << 0 << endl;
        }
        else
        {
            cout << prefixesToTopicsToCounts.at(topicAndQuery.second).at(topicAndQuery.first) << endl;
        }
    }
}

int main()
{
    // Read Topics
    string line = ReadTopicInput();

    unordered_map<string,vector<string>> topicsToTopicsOnPaths = GetTopicsToTopicsOnPaths(line);

    // Read Questions
    vector<string> questions = ReadQuestionInput();

    vector<pair<string,string>> topicsToQuestions = GetTopicsToQuestions(questions);

    unordered_map<string,unordered_map<string,int>> prefixesToTopicsToCounts = GetPrefixToTopicToCount(topicsToQuestions, topicsToTopicsOnPaths);

    // Read Queries
    vector<string> queries = ReadQueryInput();

    vector<pair<string,string>> topicsAndQueries = GetTopicsAndQueries(queries);

    SolveQueries(topicsAndQueries, prefixesToTopicsToCounts);

    return 0;
}

