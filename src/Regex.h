#ifndef REGEX_H
#define REGEX_H

#include <string>
#include <vector>
#include <stack>
#include <iostream>

namespace yak
{
    class State;
    class Edge
    {
    public:
        char label;
        State *state;

        Edge(char c);
        Edge(): Edge(0) {};
        
        Edge(char c, State *s);
        
        ~Edge() {};

        void set_final(bool is_final);
        void forward(Edge *e);
        bool is_final() const;
        
        friend std::ostream& operator<<(std::ostream& out, const Edge& e);
    };

    class State
    {
    public:
        int id;
        bool is_final;
        std::vector<Edge *> options;
        
        State(bool f, int i);
        State(): State(false, 0) {};
        
        ~State() {} ;

        void forward(Edge *e);

        friend std::ostream& operator<<(std::ostream& out, const State& s);
    };

    Edge::Edge(char c) : label(c)
    {
        state = new State();
    }

    Edge::Edge(char c, State *s): label(c), state(s) {}

    void Edge::set_final(bool is_final = true)
    {
        state->is_final = is_final;
    }

    void Edge::forward(Edge *e)
    {
        state->forward(e);
    }

    bool Edge::is_final() const
    {
        return state->is_final;
    }
    
    std::ostream& operator<<(std::ostream& out, const Edge& e)
    {
        if (0 == e.label) out << "<epsilon>";
        else out << e.label ;
        if (!e.is_final()) out << " --->";
        else out << " FIN";
        return out;
    }
    
    // State impl
    State::State(bool f, int i) : is_final(f), id(i)
    {
    }

    void State::forward(Edge *e)
    {
        options.push_back(e);
    }

    std::ostream& operator<<(std::ostream& out, const State& s)
    {
        out << s.is_final << " edges: ";
        std::for_each(s.options.begin(), s.options.end(), [&out](Edge *e){ out << e->label << " "; });

        return out;
    }
    
    class Nfa
    {
    public:
        Edge *tail; // start edge
        State *head; // ending state

        Nfa(Edge *e = nullptr, State *s = nullptr) : tail(e), head(s) {}
        ~Nfa() {};


        Nfa(std::string s)
        {
            tail = new Edge();
            Edge *e = tail;
            
            std::stack<Edge *> paths;
            paths.push(e);
                
            std::string::iterator it = s.begin();
            std::string::iterator mark = s.begin();
            while (it != s.end())
            {
                switch(*it) {
                    // case '.': // concatenate
                        
                    //     break;
                    case '|': // or
                        
                        break;
                    case '*': // 0 or more
                        
                        break;
                    case '+': // 1 or more
                        
                        break;
                    case '?': // 0 or 1
                        
                        break;
                    case '[': // begin choices
                        
                        break;
                    case ']': // end choices
                        
                        break;
                    case '(': // begin char set
                        
                        break;
                    case ')': // end char set
                        
                        break;
                    default:  // normal char
                        std::cout << "default :" << *it << std::endl;
                        Edge *newer = new Edge(*it);

                        Edge *last = (Edge *)paths.top();
                        last->forward(newer);

                        paths.pop();
                        paths.push(newer);
                        
                        break;
                }

                it++;
            }

            Edge *last = (Edge *)paths.top();
            last->set_final();
            head = last->state;
        }

        friend std::ostream& operator<<(std::ostream& out, const Nfa& nfa)
        {
            out << "label: " << *(nfa.tail) << " with state " << *(nfa.tail->state);
            return out;
        }

        void walk()
        {
            std::stack<Edge *> paths;
            Edge *e = tail;
            
            for(;;)
            {
                if (e->is_final())
                {
                    std::cout << *e << std::endl;
                    
                    if (!paths.empty())
                    {
                        // final edge
                        e = (Edge *)paths.top();
                        paths.pop();
                    }
                    else
                    {
                        break; //done
                    }
                }
                else // normal edge
                {
                    std::cout << *e << " ";
                    std::for_each(e->state->options.begin(),
                                  e->state->options.end(),
                                  [&paths](Edge *path) { paths.push(path); });

                    e = (Edge *)paths.top();
                    paths.pop();
                }
            }
        }
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
