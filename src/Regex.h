#ifndef REGEX_H
#define REGEX_H

#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <list>

namespace yak
{

    // Represents an NFA state plus zero or one or two arrows exiting.
    // if c == Match, no arrows out; matching state.
    // If c == Split, unlabeled arrows to out and out1 (if != NULL).
    // If c < 256, labeled arrow with character c to out.
    enum
    {
	Match = 256,
	Split = 257
    };

    class State
    {
    public:
        int c;
        State *out;
        State *out1;
        
        State(int c, State *out, State *out1) : c(c), out(out), out1(out1) {};
        ~State() {} ;

        friend std::ostream& operator<<(std::ostream& out, const State& s)
        {
            out << s.c;
            return out;
        }
    };
    State matchstate = { Match, nullptr, nullptr };	// matching state

    typedef union Ptrlist Ptrlist;
    struct Frag
    {
        State *start;
        Ptrlist *out;

        Frag(State *start, Ptrlist *out) : start(start), out(out) {};
        Frag() : start(nullptr), out(nullptr) {}
    };

    // Since the out pointers in the list are always 
    // uninitialized, we use the pointers themselves
    // as storage for the Ptrlists.
    union Ptrlist
    {
	Ptrlist *next;
	State *s;
    };

    // Create singleton list containing just outp.
    Ptrlist*
    list1(State **outp)
    {
	Ptrlist *l;
	
	l = (Ptrlist*)outp;
	l->next = NULL;
	return l;
    }

    // Patch the list of states at out to point to start. 
    void
    patch(Ptrlist *l, State *s)
    {
	Ptrlist *next;
	
	for(; l; l=next){
            next = l->next;
            l->s = s;
	}
    }

    // Join the two lists l1 and l2, returning the combination. 
    Ptrlist*
    append(Ptrlist *l1, Ptrlist *l2)
    {
	Ptrlist *oldl1;
	
	oldl1 = l1;
	while(l1->next)
            l1 = l1->next;
	l1->next = l2;
	return oldl1;
    }

    
    class Nfa
    {
    public:

        Nfa() {}
        ~Nfa() {};

        static State *post_to_nfa(std::string post)
        {
            if (post == "")
                return nullptr;

            std::stack<Frag> frags;
            Frag e1, e2, e;
            State *s;
            std::string::iterator it = post.begin();

            while (it != post.end())
            {
                switch(*it) {
                    default:  // normal char
                        s = new State(*it, nullptr, nullptr);
                        frags.push(Frag(s, list1(&s->out)));
                        break;
                    case '.': // concatenate
                        e2 = frags.top();
                        frags.pop();
                        e1 = frags.top();
                        frags.pop();
                        patch(e1.out, e2.start);
                        frags.push(Frag(e1.start, e2.out));
                        break;
                    case '|': // or
                        e2 = frags.top();
                        frags.pop();
                        e1 = frags.top();
                        frags.pop();
                        s = new State(Split, e1.start, e2.start);
                        frags.push(Frag(s, append(e1.out, e2.out)));
                        break;
                    case '*': // 0 or more
                        e = frags.top();
                        frags.pop();
                        s = new State(Split, e.start, nullptr);
                        patch(e.out, s);
                        frags.push(Frag(s, list1(&s->out1)));
                        break;
                    case '+': // 1 or more
                        e = frags.top();
                        frags.pop();
                        s = new State(Split, e.start, nullptr);
                        patch(e.out, s);
                        frags.push(Frag(e.start, list1(&s->out1)));
                        break;
                    case '?': // 0 or 1
                        e = frags.top();
                        frags.pop();
                        s = new State(Split, e.start, nullptr);
                        frags.push(Frag(s, append(e.out, list1(&s->out1))));
                        break;
                }
                it++;
            }

            e = frags.top();
            frags.pop();
            if (!frags.empty())
                return nullptr;

            patch(e.out, &matchstate);
            return e.start;
        }
            

        friend std::ostream& operator<<(std::ostream& out, const Nfa& nfa)
        {
            //out << "label: " << *(nfa.tail) << " with state " << *(nfa.tail->state);
            return out;
        }

        // void walk()
        // {
        //     std::stack<Edge *> paths;
        //     Edge *e = tail;
            
        //     for(;;)
        //     {
        //         if (e->is_final())
        //         {
        //             std::cout << *e << std::endl;
                    
        //             if (!paths.empty())
        //             {
        //                 // final edge
        //                 e = (Edge *)paths.top();
        //                 paths.pop();
        //             }
        //             else
        //             {
        //                 break; //done
        //             }
        //         }
        //         else // normal edge
        //         {
        //             std::cout << *e << " ";
        //             std::for_each(e->state->options.begin(),
        //                           e->state->options.end(),
        //                           [&paths](Edge *path) { paths.push(path); });

        //             e = (Edge *)paths.top();
        //             paths.pop();
        //         }
        //     }
        // }

        
    };
    
    // class Regex
    // {
    // public:
    //     Regex();
    //     ~Regex();

    // private:

    // };

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
