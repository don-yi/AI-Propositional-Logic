#include "pl.h"

CNF const operator|(Literal const& op1, Literal const& op2) { return CNF(op1) | CNF(op2); }
CNF const operator|(Literal const& op1, CNF     const& op2) { return CNF(op1) | op2; }
CNF const operator&(Literal const& op1, Literal const& op2) { return CNF(op1) & CNF(op2); }
CNF const operator&(Literal const& op1, CNF     const& op2) { return CNF(op1) & op2; }
CNF const operator>(Literal const& op1, Literal const& op2) { return CNF(op1) > CNF(op2); }
CNF const operator>(Literal const& op1, CNF     const& op2) { return CNF(op1) > op2; }

KnowledgeBase::KnowledgeBase() : clauses() {}
////////////////////////////////////////////////////////////////////////////
KnowledgeBase& KnowledgeBase::operator+=(CNF const& cnf) {
  for (std::set< Clause >::const_iterator it = cnf.begin(); it != cnf.end(); ++it) {
    clauses.insert(*it);
  }
  return *this;
}
////////////////////////////////////////////////////////////////////////
std::set< Clause >::const_iterator KnowledgeBase::begin() const { return clauses.begin(); }
std::set< Clause >::const_iterator KnowledgeBase::end()   const { return clauses.end(); }
unsigned                           KnowledgeBase::size()  const { return clauses.size(); }
////////////////////////////////////////////////////////////////////////////
bool KnowledgeBase::ProveByRefutation(CNF const& alpha) {
  //   clauses <--- the set of clauses in the CNF representation of KB ∧ ¬α
  std::set<Clause> const kb = clauses;
  CNF cnf(kb);
  cnf += ~alpha;
  if (cnf.size() == 1) return false;

  CNF prev = cnf, curr = cnf;

  //   loop do
  do
  {
    bool hasEmptyClause = false;

    CNF next = ResolveSet(prev, curr, hasEmptyClause);
    if (hasEmptyClause) return true;
    next += ResolveSet(curr, curr, hasEmptyClause);
    if (hasEmptyClause) return true;

    prev = prev & curr;
    curr = next;

    // if new ⊆ clauses then return false
    bool isSubset = false;
    for (auto&& clause : next) {
      std::set<Clause>::const_iterator findItr = cnf.Clauses().find(clause);
      if (findItr == cnf.end()) {
        isSubset = false;
        break;
      }

      isSubset = true;
    }
    if (isSubset) return false;

    cnf += next;

  } while (true);
}

CNF KnowledgeBase::ResolveSet(CNF& s1, CNF& s2, bool& hasEmptyClause) {

  CNF res;
  // for each Ci, Cj in clauses do
  for (auto && c1 : s1.Clauses()) {
    for (auto && c2 : s2.Clauses()) {
      // resolvents <----- PL-RESOLVE(Ci, Cj)
      std::set<Clause> resolvents = Resolve(c1, c2);

      // if resolvents contains the empty clause then return true
      Clause empty;
      std::set<Clause>::const_iterator findItr = resolvents.find(empty);
      if (findItr != resolvents.end()) {
        hasEmptyClause = true;
        return res;
      }

      CNF resolventCNF(resolvents);
      res += resolventCNF;
    }
  }

  return res;
}

std::set<Clause> KnowledgeBase::Resolve(Clause const& c1, Clause const& c2) {

  std::set<Clause> res; // resulting clause set

  // for all literals in both clauses
  for (auto && l1 : c1.Literals())
    for (auto&& l2 : c2.Literals()) {
      // skip non complementary
      if (not l1.Complementary(l2)) continue;

      // make resolved clause
      Clause resolved;
      ResolveClause(resolved, c1, l1);
      ResolveClause(resolved, c2, l2);

      // insert to result clause set
      res.insert(resolved);
    }

  return res;
}

// add non complementary literals to make a resolved clause
void KnowledgeBase::ResolveClause(
  Clause& resolved, Clause const& clause, Literal const& complementary) {
  for (auto&& literal : clause.Literals())
    if (not (literal == complementary)) resolved.Insert(literal);
}

////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, KnowledgeBase const& kb) {
  unsigned size = kb.clauses.size();
  for (std::set< Clause >::const_iterator it1 = kb.clauses.begin(); it1 != kb.clauses.end(); ++it1) {
    os << *it1 << ", ";
  }
  return os;
}
