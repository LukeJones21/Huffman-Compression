#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <array>
#include <cstddef>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

#include "bstream.h"
#include "pqueue.h"

class HuffmanNode {
 public:
  explicit HuffmanNode(char ch, size_t freq,
                       HuffmanNode *left = nullptr,
                       HuffmanNode *right = nullptr)
      : ch_(ch), freq_(freq), left_(left), right_(right) { }


  bool IsLeaf() {
    // Node is a leaf if it doesn't have any children
    return left_ == nullptr && right_ == nullptr;
  }

  bool operator < (const HuffmanNode &n) const {
    // In case of equality, make it deterministic based on character
    if (freq_ == n.freq_)
      return ch_ < n.ch_;
    // Otherwise compare frequencies
    return freq_ < n.freq_;
  }

  size_t freq() { return freq_; }
  size_t data() { return ch_; }
  HuffmanNode* left() { return left_; }
  HuffmanNode* right() { return right_; }

 private:
  char ch_;
  size_t freq_;
  HuffmanNode *left_, *right_;
};

class Huffman {
 public:
  static void Compress(std::ifstream &ifs, std::ofstream &ofs);

  static void Decompress(std::ifstream &ifs, std::ofstream &ofs);

 private:
  // Helper methods...
  static void PreorderRecur(HuffmanNode *n, BinaryOutputStream& bos, 
          std::string bits, std::map<char, std::string>& code_table);
};

// To be completed below
void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs) {
  int chars[128] = {0};   // All possible ASCII values initialized to freq = 0
  PQueue<HuffmanNode> pq;
  char c;
  // Counting the frequency of each ASCII char in file
  while (ifs >> c) {
    chars[c + 0]++;
  }
  // create min priority queue, ordered by frequency
  for (int i = 0; i < 128; i++) {
    if (chars[i] != 0)
      pq.Push(HuffmanNode(i, chars[i]));
  }
  // Create Huffman Tree from priority queue
  while (pq.Size() > 1) {
    HuffmanNode *n1 = new HuffmanNode(pq.Top().data(), pq.Top().freq(), pq.Top().left(), pq.Top().right());
    pq.Pop();
    HuffmanNode *n2 = new HuffmanNode(pq.Top().data(), pq.Top().freq(), pq.Top().left(), pq.Top().right());
    pq.Pop();
    pq.Push(HuffmanNode(0, n1->freq() + n2->freq(), n1, n2));
  }

  std::map<char, std::string> code_table;
  BinaryOutputStream bos(ofs);
  HuffmanNode *root = new HuffmanNode(pq.Top().data(), pq.Top().freq(),
                                     pq.Top().left(), pq.Top().right());
  // Preorder traverse Huffman Tree 
  PreorderRecur(root, bos, "", code_table);
  // Insert total amount of characters in file
  bos.PutInt(pq.Top().freq());
  ifs.clear();                 // clear fail and eof bits
  ifs.seekg(0, std::ios::beg); // Start at the beginning of the file
  // Put newly encoded characters into output file
  while (ifs >> c) {
    std::string temp = code_table.at(c);
    for (int i = 0; i < temp.length(); i++) {
      bos.PutBit(temp[i] - '0');
    }
  }
}

void Huffman::PreorderRecur(HuffmanNode *n, BinaryOutputStream& bos, 
          std::string bits, std::map<char, std::string>& code_table) {
  // If at leaf node, Put bit 1, then char value, update code_table with value
  // and delete current node
  if (n->IsLeaf()) {
    bos.PutBit(1);
    bos.PutChar(n->data());
    code_table.insert(std::pair<char,std::string>(n->data(), bits));
    delete n;
    return;
  }
  // If at internal node, Put bit 0, then recurse left and right
  // Add 0 to code table string for left and 1 for right
  bos.PutBit(0);
  PreorderRecur(n->left(), bos, bits + "0", code_table);
  PreorderRecur(n->right(), bos, bits + "1", code_table);
  // Delete node once traversed both left and right subtrees
  delete n;
}

void Huffman::Decompress(std::ifstream &ifs, std::ofstream &ofs) {
  BinaryInputStream bis(ofs);
  int counter = 0;
  bool temp = bis.GetBit();
  HuffmanNode root(0,0);
  
  do {
    // Bit == 0
    // Make Internal Node

    // Bit == 1
    // Make Leaf Node and GetChar as data of node
    temp = bis.GetBit();
    if (counter == 0) {
      
    }
    if (temp == 0) {
      counter+=2;
      while (bis.GetBit() == 0)
        counter++;
    } else {
      counter--;
      
    }
  } while(counter == 0);
}

HuffmanNode Recursion1(BinaryInputStream& bis) {
  HuffmanNode root(0,0);
  bool first_bit = bis.GetBit();
  if (first_bit == 1) {
    return HuffmanNode(bis.GetChar(), 0);
  }
  Recursion (2, &root, nullptr, bis);
}

void Recursion2(int& counter, HuffmanNode* n, HuffmanNode* parent, BinaryInputStream& bis) {
  if (counter == 0)
    return;
  
}

#endif  // HUFFMAN_H_
