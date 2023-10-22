/*******************************************************************************
 * Name        : waterjugpuzzle.cpp
 * Author      : Anna Hauk
 * Date        : 10/14/2023
 * Description : Solves the famous water jug puzzle using breadth-first search in C++
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System
 ******************************************************************************/

#include <iostream>
#include <sstream>
#include <queue>

using namespace std;

// Struct to represent state of water in the jugs.
struct State {
    int a, b, c;
    string directions;
    State *parent;
    
    State(int _a, int _b, int _c, string _directions) : 
        a{_a}, b{_b}, c{_c}, directions{_directions}, parent{nullptr}
    {

    }

    bool operator==(const State& other)
    {
        return (this->a == other.a) && (this->b == other.b) && (this->c == other.c);
    }

    // String representation of state in tuple form.
    string to_string() {
        ostringstream oss;
        oss << "(" << a << ", " << b << ", " << c << ")";
        return oss.str();
    }
};

bool check_inputs(int argc, char *argv[])
{
    istringstream iss;
    int capacityA,
        capacityB,
        capacityC,
        targetA,
        targetB,
        targetC;

    if (argc != 7)
    {
        cout << "Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>" << endl;
        return false;
    }
    iss.str(argv[1]);
    iss >> capacityA;
    if (!iss)
    {
        cout << "Error: Invalid capacity '" << argv[1] << "' for jug A." << endl;
        return false;
    }
    iss.str(argv[2]);
    iss.clear();
    iss >> capacityB;
    if (!iss)
    {
        cout << "Error: Invalid capacity '" << argv[2] << "' for jug B." << endl;
        return false;
    }

    iss.str(argv[3]);
    iss.clear();
    iss >> capacityC;
    if (!iss)
    {
        cout << "Error: Invalid capacity '" << argv[3] << "' for jug C." << endl;
        return false;
    }

    iss.str(argv[4]);
    iss.clear();
    iss >> targetA;
    if (!iss)
    {
        cout << "Error: Invalid goal '" << argv[4] << "' for jug A." << endl;
        return false;
    }
    iss.str(argv[5]);
    iss.clear();
    iss >> targetB;
    if (!iss)
    {
        cout << "Error: Invalid goal '" << argv[5] << "' for jug B." << endl;
        return false;
    }
    iss.str(argv[6]);
    iss.clear();
    iss >> targetC;
    if (!iss)
    {
        cout << "Error: Invalid goal '" << argv[6] << "' for jug C." << endl;
        return false;
    }

    if (capacityA<1)
    {
        cout << "Error: Invalid capacity '" << capacityA << "' for jug A." << endl;
        return false;
    }
    if (capacityB<1)
    {
        cout << "Error: Invalid capacity '" << capacityB << "' for jug B." << endl;
        return false;
    }
    if (capacityC<1)
    {
        cout << "Error: Invalid capacity '" << capacityC << "' for jug C." << endl;
        return false;
    }

    if (targetA<0)
    {
        cout << "Error: Invalid goal '" << targetA << "' for jug A." << endl;
        return false;
    }
    if (targetB<0)
    {
        cout << "Error: Invalid goal '" << targetB << "' for jug B." << endl;
        return false;
    }
    if (targetC<0)
    {
        cout << "Error: Invalid goal '" << targetC << "' for jug C." << endl;
        return false;
    }
    if (targetA>capacityA)
    {
        cout << "Error: Goal cannot exceed capacity of jug A." << endl;
        return false;
    }
    if (targetB>capacityB)
    {
        cout << "Error: Goal cannot exceed capacity of jug B." << endl;
        return false;
    }
    if (targetC>capacityC)
    {
        cout << "Error: Goal cannot exceed capacity of jug C." << endl;
        return false;
    }
    if (targetA+targetB+targetC != capacityC)
    {
        cout << "Error: Total gallons in goal state must be equal to the capacity of jug C." << endl;
        return false;
    }
    return true;
}

bool targetReached(State *s, int targetA, int targetB, int targetC)
{
    return (s->a==targetA) && (s->b==targetB) && (s->c==targetC);
}

bool isSeen(const vector<State*> seen, State *s)
{
    for(auto it = seen.begin(); it != seen.end(); ++it)
    {
        if (**it == *s)
        {
            return true;
        }
    }
    return false;
}

void printSolution(State *s)
{
    vector<State *> state_list;
    state_list.push_back(s);
    while (s->parent != nullptr)
    {
        s=s->parent;
        state_list.push_back(s);
    }
    for (auto it = state_list.rbegin(); it != state_list.rend(); ++it)
    {
        cout << (*it)->directions << " " << (*it)->to_string() << endl;
    }
}

int main(int argc, char *argv[]) {
    if (!check_inputs(argc,argv))
    {
        return 1;
    }
    istringstream iss;
    int capacityA,
        capacityB,
        capacityC,
        targetA,
        targetB,
        targetC;

    iss.str(argv[1]); iss.clear(); iss >> capacityA;
    iss.str(argv[2]); iss.clear(); iss >> capacityB;
    iss.str(argv[3]); iss.clear(); iss >> capacityC;
    iss.str(argv[4]); iss.clear(); iss >> targetA;
    iss.str(argv[5]); iss.clear(); iss >> targetB;
    iss.str(argv[6]); iss.clear(); iss >> targetC;
    // queue to anchor states that have been seen already
    // so we don't repeat looking at all of the bucket
    // operations when it's already been seen, i.e. reached

    vector<State*> seen;

    // sq is a queue for adding nodes at the same "level" of the breadth
    // first search tree/graph
    queue<State*> sq;
    sq.push(new State(0,0,capacityC, "Initial state."));
    bool foundSolution = false;
    while(!sq.empty())
    {
        State* s = sq.front();
        sq.pop();
        if (!isSeen(seen, s))
        {
            seen.push_back(s);
        }
        if (targetReached(s,targetA,targetB,targetC))
        {
            printSolution(s);
            foundSolution=true;
            break;
        }

        // pour from C to A
        if ((s->c>0) && (s->a < capacityA))
        {
            int a_can_take = capacityA - s->a;
            State* new_state=nullptr;
            std::stringstream ss;

            if (s->c >= a_can_take)
            {
                if (a_can_take == 1)
                {
                    ss << "Pour 1 gallon from C to A.";
                }
                else
                {
                    ss << "Pour " << a_can_take << " gallons from C to A.";
                }
                new_state = new State(capacityA,s->b,s->c-a_can_take,ss.str());
            }
            else
            {
                if (s->c == 1)
                {
                    ss << "Pour 1 gallon from C to A.";
                }
                else
                {
                    ss << "Pour " << s->c << " gallons from C to A.";
                }
                new_state = new State(s->a+s->c,s->b,0,ss.str());
            }
                
            new_state->parent=s;
            if (!isSeen(seen, new_state))
            {
                seen.push_back(new_state);
                sq.push(new_state);
            }
            else
            {
                delete new_state;
            }
        }
        // pour from B to A 
        if ((s->b>0) && (s->a < capacityA))
        {
            int a_can_take = capacityA - s->a;
            State* new_state=nullptr;
            std::stringstream ss;

            if (s->b >= a_can_take)
            {
                if (a_can_take == 1)
                {
                    ss << "Pour 1 gallon from B to A.";
                }
                else
                {
                    ss << "Pour " << a_can_take << " gallons from B to A.";
                }
                new_state = new State(capacityA,s->b-a_can_take,s->c,ss.str());
            }
            else
            {
                if (s->b == 1)
                {
                    ss << "Pour 1 gallon from B to A.";
                }
                else
                {
                    ss << "Pour " << s->b << " gallons from B to A.";
                }
                new_state = new State(s->a+s->b,0,s->c,ss.str());
            }
                
            new_state->parent=s;
            if (!isSeen(seen, new_state))
            {
                seen.push_back(new_state);
                sq.push(new_state);
            }
            else
            {
                delete new_state;
            }
        }
        // pour from C to B
        if ((s->c>0) && (s->b < capacityB))
        {
            int b_can_take = capacityB - s->b;
            State* new_state=nullptr;
            std::stringstream ss;

            if (s->c >= b_can_take)
            {
                if (b_can_take == 1)
                {
                    ss << "Pour 1 gallon from C to B.";
                }
                else
                {
                    ss << "Pour " << b_can_take << " gallons from C to B.";
                }
                new_state = new State(s->a,capacityB,s->c-b_can_take,ss.str());
            }
            else
            {
                if (s->c == 1)
                {
                    ss << "Pour 1 gallon from C to B.";
                }
                else
                {
                    ss << "Pour " << s->c << " gallons from C to B.";
                }
                new_state = new State(s->a,s->b+s->c,0,ss.str());
            }
                
            new_state->parent=s;
            if (!isSeen(seen, new_state))
            {
                seen.push_back(new_state);
                sq.push(new_state);
            }
            else
            {
                delete new_state;
            }
        }
        // pour from A to B
        if ((s->a>0) && (s->b < capacityB))
        {
            int b_can_take = capacityB - s->b;
            State* new_state=nullptr;
            std::stringstream ss;

            if (s->a >= b_can_take)
            {
                if (b_can_take == 1)
                {
                    ss << "Pour 1 gallon from A to B.";
                }
                else
                {
                    ss << "Pour " << b_can_take << " gallons from A to B.";
                }
                new_state = new State(s->a-b_can_take,capacityB,s->c,ss.str());
            }
            else
            {
                if (s->a == 1)
                {
                    ss << "Pour 1 gallon from A to B.";
                }
                else
                {
                    ss << "Pour " << s->a << " gallons from A to B.";
                }
                new_state = new State(0,s->b+s->a,s->c,ss.str());
            }
                
            new_state->parent=s;
            if (!isSeen(seen, new_state))
            {
                seen.push_back(new_state);
                sq.push(new_state);
            }
            else
            {
                delete new_state;
            }
        }
        // pour from B to C 
        if ((s->b>0) && (s->c < capacityC))
        {
            int c_can_take = capacityC - s->c;
            State* new_state=nullptr;
            std::stringstream ss;

            if (s->b >= c_can_take)
            {
                if (c_can_take == 1)
                {
                    ss << "Pour 1 gallon from B to C.";
                }
                else
                {
                    ss << "Pour " << c_can_take << " gallons from B to C.";
                }
                new_state = new State(s->a,s->b-c_can_take,capacityC,ss.str());
            }
            else
            {
                if (s->b == 1)
                {
                    ss << "Pour 1 gallon from B to C.";
                }
                else
                {
                    ss << "Pour " << s->b << " gallons from B to C.";
                }
                new_state = new State(s->a,0,s->c+s->b,ss.str());
            }
                
            new_state->parent=s;
            if (!isSeen(seen, new_state))
            {
                seen.push_back(new_state);
                sq.push(new_state);
            }
            else
            {
                delete new_state;
            }
        }
        // pour from A to C
        if ((s->a>0) && (s->c < capacityC))
        {
            int c_can_take = capacityC - s->c;
            State* new_state=nullptr;
            std::stringstream ss;

            if (s->a >= c_can_take)
            {
                if (c_can_take == 1)
                {
                    ss << "Pour 1 gallon from A to C.";
                }
                else
                {
                    ss << "Pour " << c_can_take << " gallons from A to C.";
                }
                new_state = new State(s->a-c_can_take,s->b,capacityC,ss.str());
            }
            else
            {
                if (s->a == 1)
                {
                    ss << "Pour 1 gallon from A to C.";
                }
                else
                {
                    ss << "Pour " << s->a << " gallons from A to C.";
                }
                new_state = new State(0,s->b,s->c+s->a,ss.str());
            }
                
            new_state->parent=s;
            if (!isSeen(seen, new_state))
            {
                seen.push_back(new_state);
                sq.push(new_state);
            }
            else
            {
                delete new_state;
            }
        }
    }
    if (!foundSolution)
    {
        cout << "No solution." << endl;
    }
    // the seen vector will anchor all state objects
    // allocated from the heap, so just iterate through
    // each state pointer and delete it to free up memory
    // otherwise valgrind will complain
    for(auto it = seen.begin(); it != seen.end(); ++it)
    {
        if (*it != nullptr)
        {
            delete *it;
            *it=nullptr;
        }
    }
    return 0;
}
