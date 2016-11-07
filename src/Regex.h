#ifndef REGEX_H
#define REGEX_H

#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <set>
#include <map>

namespace yak
{
    class State
    {
    public:
        std::string name;
        std::set<State *> options;
        std::map<char, State *> transitions;
        bool is_end;
        
        State(std::string n) : name(n)
        {
            is_end = false;
        };
        ~State() {} ;

        friend std::ostream& operator<<(std::ostream& out, const State& s)
        {
            out << "State:" << s.name << " is_end:" << s.is_end;
            std::cout << " transitions:";
            for (auto tran: s.transitions)
                std::cout << tran.first << " trans to " << *(tran.second) << std::endl;
            std::cout << " options:" << std::endl;
            for (auto ops: s.options)
                std::cout << *ops << std::endl;
            
            return out;
        }
    };

    
    class Nfa
    {
    public:
        State *start;
        State *end;
        
        Nfa(State *s, State *e) : start(s), end(e) { end->is_end = true; }
        ~Nfa() {};

        friend std::ostream& operator<<(std::ostream& out, const Nfa& nfa)
        {
            //out << "label: " << *(nfa.tail) << " with state " << *(nfa.tail->state);
            return out;
        }

        void add_state(State *state, std::set<State *> &state_set)
        {
            auto search = state_set.find(state);
            if (search != state_set.end())
                return;

            state_set.insert(state);
            for (auto ops: state->options)
                add_state(ops, state_set);
        }
        
        bool match(std::string s)
        {
            std::set<State *> current_states;
            add_state(start, current_states);
            
            for (auto c: s)
            {
                std::set<State *> next_states;
                for (auto state: current_states)
                {
                    auto search = state->transitions.find(c);
                    if (search != state->transitions.end())
                    {
                        State *trans_state = state->transitions[c];
                        add_state(trans_state, next_states);
                    }
                }

                current_states = next_states;

            }
            
            for (auto s: current_states)
            {
                if (s->is_end) return true;
            }

            return false;
        }

    };

    static Nfa *post_to_nfa(std::string post)
    {
        if (post == "")
            return nullptr;

        int state_count = 0;
        std::stack<Nfa *> nfa_stack;
        Nfa *nfa, *n1, *n2;
        State *s0, *s1, *s2, *s3;
        std::string::iterator it = post.begin();
        auto create_state = [&state_count]() {
            state_count += 1;
            return new State("s"+ std::to_string(state_count));
        };

        while (it != post.end())
        {
            switch(*it) {
                default:  // normal char
                    s0 = create_state();
                    s1 = create_state();
                    s0->transitions.emplace(*it, s1);
                    nfa = new Nfa(s0, s1);
                    nfa_stack.push(nfa);
                    break;
                case '.': // concatenate
                    n2 = nfa_stack.top();
                    nfa_stack.pop();
                    n1 = nfa_stack.top();
                    nfa_stack.pop();
                    n1->end->is_end = false;
                    n1->end->options.insert(n2->start);
                    nfa = new Nfa(n1->start, n2->end);
                    nfa_stack.push(nfa);
                    break;
                case '|': // or
                    n2 = nfa_stack.top();
                    nfa_stack.pop();
                    n1 = nfa_stack.top();
                    nfa_stack.pop();
                    s0 = create_state();
                    s0->options.insert(n1->start);
                    s0->options.insert(n2->start);
                    s3 = create_state();
                    n1->end->options.insert(s3);
                    n2->end->options.insert(s3);
                    n1->end->is_end = false;
                    n2->end->is_end = false;
                    nfa = new Nfa(s0, s3);
                    nfa_stack.push(nfa);
                    break;
                case '*': // 0 or more
                    n1 = nfa_stack.top();
                    nfa_stack.pop();
                    s0 = create_state();
                    s1 = create_state();
                    s0->options.insert(n1->start);
                    s0->options.insert(s1);
                    n1->end->options.insert(s1);
                    n1->end->options.insert(n1->start);
                    n1->end->is_end = false;
                    nfa = new Nfa(s0, s1);
                    nfa_stack.push(nfa);
                    break;
                case '+': // 1 or more
                    n1 = nfa_stack.top();
                    nfa_stack.pop();
                    s0 = create_state();
                    s1 = create_state();
                    s0->options.insert(n1->start);
                    n1->end->options.insert(s1);
                    n1->end->options.insert(n1->start);
                    n1->end->is_end = false;
                    nfa = new Nfa(s0, s1);
                    nfa_stack.push(nfa);
                    break;
                case '?': // 0 or 1
                    n1 = nfa_stack.top();
                    nfa_stack.pop();
                    n1->start->options.insert(n1->end);
                    nfa_stack.push(n1);
                    break;
            }
            it++;
        }


        nfa = nfa_stack.top();
        nfa_stack.pop();
        return nfa;
    }

    
    // Convert infix regexp re to postfix notation.
    // Insert . as explicit concatenation operator.
    // algorithms from russ cox.
    static
    std::string re_to_post(const std::string re)
    {
        int nalt, natom;
        struct paren {
            int nalt;
            int natom;
        } p = {0, 0};
        
        std::stack<struct paren> parens;
        std::string post;
        nalt = 0;
        natom = 0;
        
        for (auto c: re)
        {
            switch (c)
            {
                case '(':
                    if (natom > 1) {
                        --natom;
                        post += '.';
                    }

                    p.natom = natom;
                    p.nalt = nalt;
                    parens.push(p);
                    nalt = 0;
                    natom = 0;
                    break;
                case '|':
                    if (natom == 0)
                        return "";
                    while (--natom > 0)
                        post += '.';
                    nalt++;
                    break;
                case ')':
                    if (parens.empty())
                        return "";
                    if (natom == 0)
                        return "";
                    while (--natom > 0)
                        post += '.';
                    for(; nalt > 0; nalt--)
                        post += '|';
                    p = parens.top();
                    nalt = p.nalt;
                    natom = p.natom;
                    parens.pop();

                    natom++;
                    break;
                case '*':
                case '+':
                case '?':
                    if (natom == 0)
                        return "";
                    post += c;
                    break;
                default:
                    if (natom > 1) {
                        --natom;
                        post += '.';
                    }
                    post += c;
                    natom++;
                    break;
            }
        }
        if (!parens.empty())
            return "";
        while (--natom > 0)
            post += '.';
        for (; nalt > 0; nalt--)
            post += '|';
        
        return post;
    }
}
#endif
