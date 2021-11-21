#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <Windows.h>
using namespace std;
enum EColor {RED, BLACK};
struct Node
{
	int Data;
	EColor Color;
	Node* Left, * Right, * Parent;
};
typedef Node*Tree;
Tree root;
Node* createNode(int x)
{
	Node* p = new Node;
	if (p == NULL)
		return NULL;
	p->Data = x;		// Gán dữ liệu mới cho nút
	p->Color = RED;		// Gán màu đỏ (Red) mặc định
	p->Left = NULL;		// Chưa có nút con trái
	p->Right = NULL;	// Chưa có nút con phải
	p->Parent = NULL;	// Chưa có nút cha
	return p;
}
void setColor(int colorBackground, int colorText) {
	HANDLE hColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hColor, colorBackground * 16 + colorText);
	/*	0 = Black       8 = Gray
	1 = Blue        9 = Light Blue
	2 = Green       A = Light Green
	3 = Aqua        B = Light Aqua
	4 = Red         C = Light Red
	5 = Purple      D = Light Purple
	6 = Yellow      E = Light Yellow
	7 = White       F = Bright White
	=> set_Color(X); -> X = a*16 + b, a (background) và b (character)
	*/
}
void showNode(Node* p)
{
	if (p->Color == RED)
		 setColor(15, 12);	//Light Red=12 (C), Bright White=15 (F)
	else if (p->Color == BLACK)
		setColor(15, 0);	//Black=0, Bright White=15 (F)
	cout<<p->Data<<"    ";
	setColor(14, 2);		//Light Yellow=14 (E), Green=2
}
void createTree(Tree& root)
{
	root = NULL;
}
void NLR(Tree root)
{
	if (root == NULL) return;
	else {
		showNode(root);
		NLR(root->Left);
		NLR(root->Right);

	}

	
}
Node* Insert(Tree root, Node* p, int& flag)
{

	/* Nếu cây trống thì trả về một BRTNode mới */
	if (root == NULL)
	{
		root = p;
		flag = 1;
		return root;
	}

	/* Nếu không thì tiếp tục duyệt xuống dưới cây */
	if (p->Data < root->Data)
	{
		root->Left = Insert(root->Left, p, flag);
		root->Left->Parent = root;
	}
	else
		if (p->Data > root->Data)
		{
			root->Right = Insert(root->Right, p, flag);
			root->Right->Parent = root;
		}
		else
			flag = 0;
	return root;
}
void rotateLeft(Tree& root, Node*& p)
{
	Node* pRight = p->Right;

	p->Right = pRight->Left;

	if (p->Right != NULL)
		p->Right->Parent = p;

	pRight->Parent = p->Parent;

	if (p->Parent == NULL)
		root = pRight;

	else if (p == p->Parent->Left)
		p->Parent->Left = pRight;

	else
		p->Parent->Right = pRight;

	pRight->Left = p;
	p->Parent = pRight;
}

/* Thuật toán xoay phải */
void rotateRight(Tree& root, Node*& p)
{
	Node* pLeft = p->Left;

	p->Left = pLeft->Right;

	if (p->Left != NULL)
		p->Left->Parent = p;

	pLeft->Parent = p->Parent;

	if (p->Parent == NULL)
		root = pLeft;

	else if (p == p->Parent->Left)
		p->Parent->Left = pLeft;

	else
		p->Parent->Right = pLeft;

	pLeft->Right = p;
	p->Parent = pLeft;
}
void fixViolation(Tree & root, Node*& pt)
{
	Node* pParent = NULL;
	Node* pGrandParent = NULL;

	while ((pt != root) && (pt->Color != BLACK) && (pt->Parent->Color == RED))
	{
		pParent = pt->Parent;
		pGrandParent = pt->Parent->Parent;

		/* Trường hợp A:
		node cha của pt là con trái của node cha của pt */
		if (pParent == pGrandParent->Left)
		{
			Node* pUncle = pGrandParent->Right;

			/* Trường hợp: 1
			node chú của pt là node đỏ khi này chỉ cần đổi màu cho node đó thành đen */
			if (pUncle != NULL && pUncle->Color == RED)
			{
				pGrandParent->Color = RED;
				pParent->Color = BLACK;
				pUncle->Color = BLACK;
				pt = pGrandParent;
			}

			else
			{
				/* Trường hợp: 2
				pt là node con phải của node cha nó ta thực hiện xoay trái */
				if (pt == pParent->Right)
				{
					rotateLeft(root, pParent);
					pt = pParent;
					pParent = pt->Parent;
				}

				/* Trường hợp: 3
				pt là con trái của node cha nó ta thực hiện xoay phải */
				rotateRight(root, pGrandParent);
				swap(pParent->Color, pGrandParent->Color);
				pt = pParent;
			}
		}

		/* Trường hợp: B
		node cha của pt là con phải của node cha của pt */
		else
		{
			Node* pUncle = pGrandParent->Left;

			/* Trường hợp: 1
			node chú của pt là node đỏ khi này chỉ cần đổi màu cho node đó thành đen */
			if ((pUncle != NULL) && (pUncle->Color == RED))
			{
				pGrandParent->Color = RED;
				pParent->Color = BLACK;
				pUncle->Color = BLACK;
				pt = pGrandParent;
			}
			else
			{
				/* Trường hợp: 2
				pt là con trái của node cha nó ta thực hiện xoay phải */
				if (pt == pParent->Left)
				{
					rotateRight(root, pParent);
					pt = pParent;
					pParent = pt->Parent;
				}

				/* Trường hợp: 3
				pt là node con phải của node cha nó -> nên thực hiện xoay trái */
				rotateLeft(root, pGrandParent);
				swap(pParent->Color, pGrandParent->Color);
				pt = pParent;
			}
		}
	}

	root->Color = BLACK;
}
void insert(Node*& root, int Data)
{
	int flag = 0;
	Node* pNew = createNode(Data);

	/* Thực hiện chèn như bình thường */
	root = Insert(root, pNew, flag);

	/* Sửa lại lỗi của quy tắc cây đỏ đen */
	if (flag)
		fixViolation(root, pNew);
}
void createTreeArray(Tree& root,int a[],int n)
{
	createTree(root);
	for (int i = 0; i < n; i++)
	{
		insert(root, a[i]);
		setColor(14, 2);
		cout << "\nSau khi them " << a[i];
		NLR(root);
	}
}
void createTreeFromFile(Tree& root, string filename)
{
	ifstream f;
	f.open(filename);
	if (f.eof()) cout << "Không tìm thấy file";
	else
	{
		int n;
		int x;
		f >> n;
		createTree(root);
		for (int i = 1; i <= n; i++)
		{
			f >> x;
			insert(root, x);
		}
	}
	f.close();
}
void BFS(Tree root)//duyet theo chieu rong
{
	if (!root) return;
	queue<Node*> q;
	q.push(root);
	while (!q.empty())
	{
		Node* p = q.front();
		q.pop();
		showNode(p);
		if (p->Left != NULL)
			q.push(p->Left);
		if (p->Right != NULL)
			q.push(p->Right);
	}
}
void DFS(Tree root)//duyet theo chieu sau
{
	if (!root) return;
	stack<Node*> q;
	q.push(root);
	while (!q.empty())
	{
		Node* p = q.top();
		q.pop();
		showNode(p);
		if (p->Left != NULL)
			q.push(p->Left);
		if (p->Right != NULL)
			q.push(p->Right);
	}
}
void findNodeReplace(Tree& root, Tree q)
{//Node q là node thế mạng
	if (q->Left != NULL)//duyệt bên trái nhất
		findNodeReplace(root, q->Left);//tìm node trái nhất
	else
	{
		root->Data = q->Data;//cập nhật data node cần xóa chính là node thế mạng
		root = q;//trỏ root đến node thế mạng
		q = q->Right;//cập nhật mối liên kết
	}
}
void deleteNode(Tree& root, int x)
{
	if (root == NULL)
		return;
	else
	{
		if (root->Data > x)
			deleteNode(root->Left, x);
		else if (root->Data < x)
			deleteNode(root->Right, x);
		else
		{
			Node* p = root;
			if (root->Left == NULL)
				root = root->Right;
			else if (root->Right == NULL)
				root = root->Left;
			else
				findNodeReplace(root, p);
			delete p;
		}
	}
}
Node* findNode(Tree root, int x)
{
	if (root == NULL) return NULL;
	else
	{
		if (root->Data > x)
			findNode(root->Left, x);
		else if (root->Data < x)
			findNode(root->Right, x);
		else
			return root;
	}
}
int hasRedChild(Node* p)
{
	if (p->Left != NULL && p->Left->Color == RED|| p->Right != NULL && p->Right->Color == RED) return 1;
	return 0;
}
int countRedNode(Tree root)
{
	if (root)
	{
		int countL = countRedNode(root->Left);
		int countR = countRedNode(root->Right);
		if (root->Color == RED)
			return (1 + countL + countR);
		return countL + countR;
	}
	else
		return 0;
}
int countNode(Tree root)
{
	if (root)
	{
		return 1 + countNode(root->Left) + countNode(root->Right);
	}
	else return 0;
}
int countNodeK(Tree root, int k)
{
	if (root)
	{
		k--;
		int a = countNodeK(root->Left, k);
		int b = countNodeK(root->Right, k);
		if (k == 0)
			return 1 + a + b;
		return a + b;
	}
	return 0;
}
int countRedNodeK(Tree root, int k)
{
	if (root)
	{
		k--;
		int a = countRedNodeK(root->Left, k);
		int b = countRedNodeK(root->Right, k);
		if (root->Color==RED&&k==0)
			
				return 1 + a + b;
				return a + b;
	}
	return 0;
}
void tongGiaTriNut(Tree root)
{
	int sum = 0;
	if (root!=NULL)
	{
		sum+=root->Data;
		tongGiaTriNut(root->Left);
		tongGiaTriNut(root->Right);
	}
	else
		return ;;
}
//void tinhTongGiaTri()
//{
//	int sum = 0;
//	while(tongGiaTriNut())
//}
void menu()
{
	//system("cls");
	cout << "\n1. Tao cay tu mang";
	cout << "\n2. Tao cay bang doc file ";
	cout << "\n3. Duyet NLR";
	cout << "\n4. Duyet cay theo chieu sau NLR";
	cout << "\n5. Duyet cay theo chieu rong NLR";
	cout << "\n6. Them nut";
	cout << "\n7. Xoa nut";
	cout << "\n8. Tim nut";
	cout << "\n9. Dem nut do, den";
	cout << "\n10. Dem nut do den tren muc k";
	cout << "\n 11. Tinh tong cac gia tri nut tren muc k";
}
int main()
{
	Tree root;
	createTree(root);
	int a[50];
	int chucnang;
	do
	{
		menu();
		cout << "\nChon chuc nang: ";
		cin >> chucnang;
		switch (chucnang)
		{
		case 1:
		{
			int a[] = { 4, 7, 12, 15, 3, 5, 14, 18, 16, 17,20,21,30,1,11,10,9,8,24 };
			int n = 19;
			createTreeArray(root, a, n); break;
		}
		case 2:
		{
			string filename = "file.txt";
			createTreeFromFile(root, filename);
			break;
		}
		case 3:
		{
			setColor(14, 2);
			cout << "\nNLR: ";
			NLR(root); break;
		}
		case 4:
		{
			cout << "\nDFS NLR";
			DFS(root);
			break;
		}
		case 5:
		{
			cout << "\nBFS NLR:";
			BFS(root);
			break;
		}
		case 6:
		{
			int y;
			cout << "Nhap nut can them: ";
			cin >> y;
			insert(root, y);
			NLR(root);
			break;
		}
		case 7:
		{
			int x;
			cout << "Nhap nut can xoa: ";
			cin >> x;
			deleteNode(root, x); 
			NLR(root);
			break;
		}
		case 8:
		{
			int x;
			cout << "Nhap nut can tim: ";
			cin >> x;
			Node* p = findNode(root, x);
			if (p == NULL)
				cout << "Không tìm thấy nút";
			else
				cout << "Tìm thấy nút";
			break;
		}
		case 9:
		{
			int nutdo = countRedNode(root);
			cout << "\nSo nut do: " << nutdo;
			cout << "\nSo nut den: " << countNode(root) - nutdo; break;
		}
		case 10:
		{
			int k;
			cout << "\nNhap muc k: ";
			cin >> k;
			int nutdo = countRedNodeK(root, k);
			int tong = countNodeK(root, k);
			cout << "\nTong so nut: " << tong;
			cout << "\nSo nut do: " << nutdo;
			cout << "\nSo nut den: " << tong-nutdo;
		}
		case 11:
		{
			tongGiaTriNut(root);

		}
		default:
			break;
		}
	} while (chucnang!=0);
	return 0;

}