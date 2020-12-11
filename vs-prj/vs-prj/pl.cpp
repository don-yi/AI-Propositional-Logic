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

  //   new <--- {}
  CNF newC;

  //   loop do
  do
  {
  //      for each Ci, Cj in clauses do
    for (std::set<Clause>::const_iterator i = cnf.begin();
      i != std::next(cnf.end(), -1);
      ++i) {
      for (std::set<Clause>::const_iterator j = std::next(i);
        j != cnf.end();
        ++j) {
        //          resolvents <----- PL-RESOLVE(Ci, Cj)
        std::set<Clause> resolvents = Resolve(*i, *j);

        ////          if resolvents contains the empty clause then return true
        //for (auto&& resolvent : resolvents)
        //  if (not resolvent.size()) return true;

        //          if resolvents contains the empty clause then return true
        Clause empty;
        std::set<Clause>::const_iterator findItr = resolvents.find(empty);
        if (findItr != resolvents.end())
        {
          return true;
        }

        //          new <--- new ∪ resolvents
        CNF resC(resolvents);
        newC += resC;
      }
    }

    //      if new ⊆ clauses then return false
    bool isSubset = false;
    for (auto&& newc : newC) {
      std::set<Clause>::const_iterator findItr = cnf.Clauses().find(newc);
      if (findItr == cnf.end()) {
        isSubset = false;
        break;
      }

      isSubset = true;
    }
    if (isSubset) return false;

    //      clauses <---- clauses  ∪ new
    cnf += newC;

  } while (true);
}

std::set<Clause> KnowledgeBase::Resolve(
  Clause const& c1, Clause const& c2) {

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
