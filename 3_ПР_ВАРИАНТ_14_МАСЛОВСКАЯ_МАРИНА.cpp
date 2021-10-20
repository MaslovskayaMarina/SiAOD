#include<iostream>
#include<stack>
using namespace std;

struct Node
{
    char data;
    Node* left, * right;
};

bool isOperator(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') return true;
    else return false;
}

Node* newNode(char data)
{
    Node* node = new Node;
    node->left = node->right = NULL;
    node->data = data;
    return node;
};

Node* constructTree(char postfix[])
{
    stack<Node*> st;
    Node* x, * y, * z;
    for (int i = 0; i < strlen(postfix); i++)
    {
        if (!isOperator(postfix[i]))
        {
            x = newNode(postfix[i]);
            st.push(x);
        }
        else
        {
            x = newNode(postfix[i]);
            y = st.top();
            st.pop();
            z = st.top();
            st.pop();
            x->right = y;
            x->left = z;
            st.push(x);
        }
    }
    x = st.top();
    st.pop();
    return x;
}

int priority(char x)
{
    if ((x == '*') || (x == '/')) return 2;
    if ((x == '+') || (x == '-')) return 1;
    if ((x == '(') || (x == ')')) return 0;
}

class stack1
{
    int top;
    char body[100];
public:
    stack1() {
        top = NULL;
    }
    bool empty() {
        return top == NULL;
    }
    char get_top_element() {
        return body[top];
    }
    int top_prior() {
        return priority(body[top]);
    }
    void push(char x)
    {
        top++;
        body[top] = x;
    }
    char pop()
    {
        top--;
        return body[top + 1];
    }
};

string convertToPostfix(char *data) {
    stack1 s;
    int k = 0;
    char NewData[100];
    int len = int(strlen(data));
    for (int i = 0; i <= len-1; i++)
    {
        if (data[i] == '(') s.push(data[i]);
        else if ((data[i] == '+') || (data[i] == '-') || (data[i] == '/') || (data[i] == '*'))
        {
            while ((!s.empty()) && (s.top_prior() > priority(data[i])))
            {
                k++;
                NewData[k] = s.pop();
            }
            s.push(data[i]);
        }
        else if (data[i] == ')')
        {
            while ((!s.empty()) && (s.get_top_element() != '('))
            {
                k++;
                NewData[k] = s.pop();
            }
            s.pop();
        }
        else
        {
            k++;
            NewData[k] = data[i];
        }
    }
    while (!s.empty())
    {
        k++;
        NewData[k] = s.pop();
    }
    string NewDataInString;
    for (int i = 1; i <= k; i++)
        NewDataInString = NewDataInString + NewData[i];
    return NewDataInString;
}

bool isEmpty(Node* T) {
    if (T) return false;
    else return true;
}

int printBinTree(Node* T, int level, int L) {
    if (!isEmpty(T->right)) {
        printBinTree(T->right, level + 1, L);
        for (int i = 1; i <= level * L; i++) {
            cout << ' ';
        }
        cout << T->data << endl;
    }
    if (!isEmpty(T->left)) {
        printBinTree(T->left, level + 1, L);
    }
    else {
        for (int i = 1; i <= level * L; i++) {
            cout << ' ';
        }
        cout << T->data << endl;
    }
    return 0;
}

int nodes(Node* T, int level, int i, int k) {
    if (i == level) k++;
    if (!isEmpty(T->right)) {
        k += nodes(T->right, level, i + 1, 0);

    }
    if (!isEmpty(T->left)) {

        k += nodes(T->left, level, i + 1, 0);
    }
    return k;
}

void infix(Node* T)
{
    if (T)
    {
        cout << T->data;
        infix(T->left);    
        infix(T->right);
    }
}

double leftproblem(Node* T) {
    if (T->left != NULL and T->right != NULL) {
            if (T->data == '+') return leftproblem(T->left) + leftproblem(T->right);
            if (T->data == '-') return leftproblem(T->left) - leftproblem(T->right);
            if (T->data == '*') return leftproblem(T->left) * leftproblem(T->right);
            if (T->data == '/') return leftproblem(T->left) / leftproblem(T->right);
            }
    return (T->data)-48;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    char str[] = "4*(1+2)+(2*2)";
    cout << "������� ��������� " << str << endl;
    string postfix = convertToPostfix(str);
    char char_array[10];
    for (int i = 0; i < 10; i++)
        char_array[i] = postfix[i];
    Node* root = constructTree(char_array);
    while (true) {
        int p;
        cout << "-----����-----" << endl;
        cout << "1 - ����� ������ �� �����" << endl;
        cout << "2 - ������� ���������� ����� �� �������� ������" << endl;
        cout << "3 - ��������� �������� ��������� � ����� ���������" << endl;
        cout << "4 - ����� ���������� ����� ������ ���������" << endl;
        cout << "0 - ����� �� ���������" << endl;
        cin >> p;
        if (p == 0) break;
        if (p == 1) {
            cout << "-----����� ������-----" << endl;
            printBinTree(root, 0, 1);
        }
        if (p == 2) {
            cout << "-----����ר� ���������� ����� �� �������� ������-----" << endl;
            int level;
            cin >> level;
            cout << "���������� ����� = " << nodes(root, level, 0, 0) << endl;
        }
        if (p == 3) {
            cout << "-----���������� �������� ��������� � ����� ���������-----" << endl;
            cout << "��������� = " << leftproblem(root->left) << endl;
        }
        if (p == 4) {
            cout << "-----����� ���������� ����� ������ ���������-----" << endl;
            cout << "���������� ����� ���������: ";
            infix(root->left);
            cout << endl;
        }
    }
    return 0;
}