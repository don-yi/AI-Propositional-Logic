#ifndef PL4_H
#define PL4_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

class Literal {
public:
  Literal(std::string const& _name) : name(_name), negated(false) { }
  Literal() : name(""), negated(false) { } // just for map.operator[]
  ////////////////////////////////////////////////////////////////////////
  Literal& Negate() {
    negated = not negated;
    return *this;
  }
  bool IsPositive() const { return !negated; }
  ////////////////////////////////////////////////////////////////////////
  bool operator==(Literal const& op2) const {
    Literal const& op1 = *this;
    return (op1.negated == op2.negated) && (op1.name == op2.name);
  }
  ////////////////////////////////////////////////////////////////////////
  bool operator<(Literal const& op2) const {
    Literal const& op1 = *this;
    //negated infront
    if (op1.negated && !op2.negated) {
      return true;
    }
    if (!op1.negated && op2.negated) {
      return false;
    }
    return (op1.name < op2.name);
  }
  ////////////////////////////////////////////////////////////////////////
  Literal operator~() const {
    Literal result(*this);
    result.Negate();
    return result;
  }
  ////////////////////////////////////////////////////////////////////////
  bool Complementary(Literal const& op2) const {
    return (name == op2.name) && (negated != op2.negated);
  }
  ////////////////////////////////////////////////////////////////////////
  friend std::ostream& operator<<(std::ostream& os, Literal const& literal) {
    os << (literal.negated ? "~" : "") << literal.name;
    return os;
  }
  ////////////////////////////////////////////////////////////////////////
  unsigned size() const {
    return lSize;
  }
private:
  std::string name;
  bool negated;
  unsigned lSize;

};

class Clause {
public:
  Clause() : literals({}), cSize(0) {}
  Clause(Literal const& literal) : cSize(0) {
    literals.insert(literal);
    ++cSize;
  }
  ////////////////////////////////////////////////////////////////////////
  unsigned size() const { return cSize; }
  ////////////////////////////////////////////////////////////////////////
  void Insert(Literal const& literal) {
    literals.insert(literal);
    ++cSize;
  }
  ////////////////////////////////////////////////////////////////////////
  void Negate() {
    for (auto literal : literals) {
      Literal placeholder = literal.Negate();
    }
  }
  ////////////////////////////////////////////////////////////////////////
  std::set<Literal> Literals() const { return literals; }
  ////////////////////////////////////////////////////////////////////////
  // ..........
  // ..........
  // ..........
  // ..........
  ////////////////////////////////////////////////////////////////////////
  // TODO: rm this fn
  bool operator<(Clause const& clause) const {
    return false; // placeholer
  }
  ////////////////////////////////////////////////////////////////////////
  friend std::ostream& operator<<(std::ostream& os, Clause const& clause) {
    unsigned size = clause.literals.size();

    if (clause.size() == 0) {
      os << " FALSE ";
    }
    else {
      std::set< Literal >::const_iterator it = clause.literals.begin();
      os << "( " << *it;
      ++it;
      for (; it != clause.literals.end(); ++it) {
        os << " | " << *it;
      }
      os << " ) ";
    }
    return os;
  }
private:
  std::set<Literal> literals;
  unsigned cSize;

};

class CNF {
public:
  // TODO
  CNF() : clauses() { }
  CNF(Literal const& literal) {
    Clause clause(literal);
    clauses.insert(clause);
  }
  ////////////////////////////////////////////////////////////////////////
  // ..........
  // ..........
  // ..........
  // ..........
  ////////////////////////////////////////////////////////////////////////
  // not
  CNF const operator~() const {
    CNF res;

    //if CNF is made of a single clause: A | B | ~C,
    if (size() == 1) {
      //negating it gives ~A & ~B & C (3 clauses)
      Clause clause = *begin();
      for (auto&& literal : clause.Literals())
      {
        Clause newClause(~literal);
        res.clauses.insert(newClause);
      }

    //otherwise
    } else {
      //CNF = clause1 & clause2 & clause3,
      //~CNF = ~clause1 | ~clause2 | ~clause3 
      //"or" is defined later 
    }
    return res;
  }
  ////////////////////////////////////////////////////////////////////////
  // =>
  CNF const operator>(CNF const& op2) const {
    CNF const& op1 = *this;
    return ~(op1) | op2;
  }
  ////////////////////////////////////////////////////////////////////////
  // and
  CNF const operator&(CNF const& op2) const {
    // TODO
    //CNF1 = clause1 & clause2 & clause3,
    //CNF2 = clause4 & clause5 & clause6,
    //CNF1 & CNF2 = clause1 & clause2 & clause3 & clause4 & clause5 & clause6
    return {};
  }
  ///////////////////////////////////////////////////////////////////////
  // or
  CNF const operator|(CNF const& op2) const {
    CNF res;

    //CNF1 = clause1 & clause2 & clause3,
    //CNF2 = clause4 & clause5 & clause6,
    //CNF1 | CNF2 = 
    //              c1|c4 & c1|c5 & c1|c6    &
    //              c2|c4 & c2|c5 & c2|c6    &
    //              c3|c4 & c3|c5 & c3|c6
    for (std::set<Clause>::const_iterator i = begin(); i != end(); ++i) {
      for (
        std::set<Clause>::const_iterator j = op2.begin(); j != op2.end(); ++j) {
        Clause clause;
        for (Literal literal : i->Literals()) clause.Insert(literal);
        for (Literal literal : j->Literals()) clause.Insert(literal);
        res.clauses.insert(clause);
      }
    }

    return res;
  }

  /////////////////////////////////////////////////////////////////////////////////
  CNF const operator>(Literal const& op2) const { return operator>(CNF(op2)); }
  CNF const operator&(Literal const& op2) const { return operator&(CNF(op2)); }
  CNF const operator|(Literal const& op2) const { return operator|(CNF(op2)); }

  ////////////////////////////////////////////////////////////////////////
  bool Empty() const { return clauses.size() == 0; }
  ////////////////////////////////////////////////////////////////////////
  std::set<Clause>::const_iterator begin() const { return clauses.begin(); }
  std::set<Clause>::const_iterator end()   const { return clauses.end(); }
  unsigned                           size()  const { return clauses.size(); }
  ////////////////////////////////////////////////////////////////////////
  friend std::ostream& operator<<(std::ostream& os, CNF const& cnf) {
    unsigned size = cnf.clauses.size();
    for (std::set< Clause >::const_iterator it1 = cnf.clauses.begin(); it1 != cnf.clauses.end(); ++it1) {
      os << *it1 << ", ";
    }
    return os;
  }

private:
  std::set<Clause> clauses;

};

CNF const operator|(Literal const& op1, Literal const& op2);
CNF const operator|(Literal const& op1, CNF     const& op2);
CNF const operator&(Literal const& op1, Literal const& op2);
CNF const operator&(Literal const& op1, CNF     const& op2);
CNF const operator>(Literal const& op1, Literal const& op2);
CNF const operator>(Literal const& op1, CNF     const& op2);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class KnowledgeBase {
public:
  ////////////////////////////////////////////////////////////////////////////
  KnowledgeBase();
  ////////////////////////////////////////////////////////////////////////////
  KnowledgeBase& operator+=(CNF const& cnf);
  ////////////////////////////////////////////////////////////////////////
  std::set< Clause >::const_iterator begin() const;
  std::set< Clause >::const_iterator end()   const;
  unsigned                           size()  const;
  ////////////////////////////////////////////////////////////////////////////
  bool ProveByRefutation(CNF const& alpha) const;
  ////////////////////////////////////////////////////////////////////////////
  friend std::ostream& operator<<(std::ostream& os, KnowledgeBase const& kb);
private:
  std::set< Clause > clauses;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
