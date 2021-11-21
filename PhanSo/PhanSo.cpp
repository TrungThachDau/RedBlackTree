#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <windows.h>
using namespace std;
struct PhanSo
{
	int tu, mau;
};
typedef PhanSo ItemType;
enum EColor{BLACK, RED};
struct Node
{
	ItemType Data;
	EColor Color;
	Node* Left, * Right, * Parent;
};
typedef Node* Tree;
Tree root;
Node* createNode(ItemType x)
{
	Node* p = new Node;
	if (p == NULL)
	{
		return NULL;
	}
	p->Data = x;		// Gán dữ liệu mới cho nút
	p->Color = RED;		// Gán màu đỏ (Red) mặc định
	p->Left = NULL;		// Chưa có nút con trái
	p->Right = NULL;	// Chưa có nút con phải
	p->Parent = NULL;	// Chưa có nút cha
	return p;
}
void set_Color(int colorBackground, int colorText) {
	HANDLE hColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hColor, colorBackground * 16 + colorText);
	/*
	0 = Black       8 = Gray
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
		set_Color(15, 12);	//Light Red=12 (C), Bright White=15 (F)
	else if (p->Color == BLACK)
		set_Color(15, 0);	//Black=0, Bright White=15 (F)
	printf("%d/%d ", p->Data.tu, p->Data.mau);
	set_Color(14, 2);		//Light Yellow=14 (E), Green=2
}
void createTree(Tree& root)
{
	root = NULL;
}
int soSanhPhanSo(ItemType p1, ItemType p2)
{
	if (p1.tu == p2.tu && p1.mau == p2.mau)  //nếu (p1 == p2)
		return 0;

	double gt = p1.tu * p2.mau - p1.mau * p2.tu;
	if (gt > 0)
		return 1;	//phân số 1 lớn hơn phân số 2 ( p1 > p2 )
	else
		return -1;	//phân số 1 nhỏ hơn phân số 2  ( p1 < p2 )
}
Node* Insert(Node* root, Node* p)
{
	/* Nếu cây trống thì trả về một BRTNode mới */
	if (root == NULL) return p;

	/* Nếu không thì tiếp tục duyệt xuống dưới cây */
	if (soSanhPhanSo(p->Data, root->Data) < 0)
	{
		root->Left = Insert(root->Left, p);
		root->Left->Parent = root;
	}
	else if (soSanhPhanSo(p->Data, root->Data) > 0)
	{
		root->Right = Insert(root->Right, p);
		root->Right->Parent = root;
	}

	/* Trả về con trỏ BRTNode */
	return root;
}
void NLR(Tree root)
{
	if (root == NULL) return;

	showNode(root); //printf("%4d", root->Info);
	NLR(root->Left);
	NLR(root->Right);
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
void rotateRight(Tree root, Node*& p)
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
/* Sửa lại cấu trúc khi chèn BRTNode vào hoặc xóa node */
void fixViolation(Tree& root, Node*& pt)
{
	Node* pParent = NULL;
	Node* pGrandParent = NULL;

	if (!pt || !pt->Parent) return; //Nút rỗng thì dừng

	while ((pt != root) && (pt->Color != BLACK) && (pt->Parent->Color == RED))
	{
		pParent = pt->Parent;
		pGrandParent = pt->Parent->Parent; //Nút ông

		if (!pGrandParent) break; //Nút ông bị rỗng

		/* Trường hợp A:
		node cha của pt là con trái của node ông của pt */
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
void addNode(Node*& root, ItemType Data)
{
	Node* pNew = createNode(Data);

	/* Thực hiện chèn như bình thường */
	root = Insert(root, pNew);

	/* Sửa lại lỗi của quy tắc cây đỏ đen */
	fixViolation(root, pNew);
}
void NLR_BFS(Tree root) 
{
	if (root == NULL) return;

	queue <Node*> q;
	q.push(root);

	while (!q.empty()) {
		Node* p;
		p = q.front();
		q.pop();
		showNode(p);
		if (p->Left != NULL)
			q.push(p->Left);
		if (p->Right != NULL)
			q.push(p->Right);
	}
}
Node* search(Node* root, ItemType x) {
	Node* temp = root;
	while (temp != NULL) {
		if (soSanhPhanSo(x, temp->Data) == 0) {
			return temp;
		}
		else if (soSanhPhanSo(x, temp->Data) < 0) {
			temp = temp->Left;
		}
		else {
			temp = temp->Right;
		}
	}

	return NULL; //Không tìm thấy x trong cây
}
void NLR_DFS(Tree root) 
{
	if (root == NULL) return;

	stack <Node*> s;
	s.push(root);

	while (!s.empty()) {
		Node* p;
		p = s.top();
		s.pop();
		showNode(p);
		//right child is pushed first so that left is processed first
		if (p->Right != NULL)
			s.push(p->Right);
		if (p->Left != NULL)
			s.push(p->Left);
	}
}
/* Kiểm tra xem node hiện tại có phải là node con trái của node cha không */
bool isOnLeft(Node* p)
{
	return p == p->Parent->Left;
}

/* Trả về con trỏ tới node chú (Uncle) */
Node* findUncle(Node* p)
{
	// Nếu không có node cha hoặc node ông, thì không có node chú
	if (p->Parent == NULL || p->Parent->Parent == NULL)
		return NULL;

	if (isOnLeft(p->Parent))
		// node chú bên phải
		return p->Parent->Parent->Right;
	else
		// node chú bên trái
		return p->Parent->Parent->Left;
}

/* Trả về con trỏ cho node anh chị em */
Node* findSibling(Node* p) {
	// node anh rỗng nếu không tồn tại node cha 
	if (p->Parent == NULL)
		return NULL;

	if (isOnLeft(p))
		return p->Parent->Right;
	else
		return p->Parent->Left;
}
/* Tìm nút không có nút con bên trái trong cây con của nút đã cho */
Node* findSuccessor(Node* p) 
{
	Node* temp = p;

	while (temp->Left != NULL)
		temp = temp->Left;

	return temp;
}

/* Kiểm tra có node hiện tại có node con là nút đỏ hay không */
bool hasRedChild(Node* p) 
{
	return (p->Left != NULL && p->Left->Color == RED) ||
		(p->Right != NULL && p->Right->Color == RED);
}

void fixDoubleBlack(Tree& root, Node* p) {
	// p là node gốc thì return
	if (p == root) return;

	Node* pSibling = findSibling(p);
	Node* pParent = p->Parent;
	if (pSibling == NULL) {
		// Không có sibiling, màu đen kép được đẩy lên 
		fixDoubleBlack(root, pParent);
	}
	else {
		if (pSibling->Color == RED) {
			// Anh chị em màu đỏ 
			pParent->Color = RED;
			pSibling->Color = BLACK;
			if (isOnLeft(pSibling)) {
				// trường hợp left 
				rotateRight(root, pParent);
			}
			else {
				// trường hợp right 
				rotateLeft(root, pParent);
			}
			fixDoubleBlack(root, p);
		}
		else {
			// Anh chị em đen 
			if (hasRedChild(pSibling)) {
				// Ist nhất 1 trẻ em màu đỏ 
				if (pSibling->Left != NULL && pSibling->Left->Color == RED) {
					if (isOnLeft(pSibling)) {
						// left left 
						pSibling->Left->Color = pSibling->Color;
						pSibling->Color = pParent->Color;
						rotateRight(root, pParent);
					}
					else {
						// right left 
						pSibling->Left->Color = pParent->Color;
						rotateRight(root, pSibling);
						rotateLeft(root, pParent);
					}
				}
				else {
					if (isOnLeft(pSibling)) {
						// left right 
						pSibling->Right->Color = pParent->Color;
						rotateLeft(root, pSibling);
						rotateRight(root, pParent);
					}
					else {
						// right right 
						pSibling->Right->Color = pSibling->Color;
						pSibling->Color = pParent->Color;
						rotateLeft(root, pParent);
					}
				}
				pParent->Color = BLACK;
			}
			else {
				// Hai con đen 
				pSibling->Color = RED;
				if (pParent->Color == BLACK)
					fixDoubleBlack(root, pParent);
				else
					pParent->Color = BLACK;
			}
		}
	}
}
Node* findNodeReplace(Node* p) {
	// Khi nút có 2 con
	if (p->Left != NULL && p->Right != NULL)
		return findSuccessor(p->Right);

	// Khi node lá 
	if (p->Left == NULL && p->Right == NULL)
		return NULL;

	// Khi node có một con
	if (p->Left != NULL)
		return p->Left;
	else
		return p->Right;
}
//void deleteNodeX(Tree& root, ItemType x)
//{
//	if (root == NULL)
//		return;
//	else
//	{
//		if (root->Data > x)
//			deleteNodeX(root->Left, x);
//		else if (root->Data < x)
//			deleteNodeX(root->Right, x);
//		else
//		{
//			Node* p = root;
//			if (root->Left == NULL)
//				root = root->Right;
//			else if (root->Right == NULL)
//				root = root->Left;
//			else
//				findNodeReplace(root, p);
//			delete p;
//		}
//	}
//}
void deleteNode(Tree& root, Node* pDelete) {
	Node* pReplace = findNodeReplace(pDelete);

	// Đúng khi pReplace và pDelete đều đen
	bool flagDoubleBlack = ((pReplace == NULL || pReplace->Color == BLACK) && (pDelete->Color == BLACK));
	Node* pParent = pDelete->Parent;

	if (pReplace == NULL) {
		// pReplace là NULL do đó pDelete là lá 
		if (pDelete == root) {
			// pDelete là root, làm cho root là NULL 
			root = NULL;
		}
		else {
			if (flagDoubleBlack) {
				// pReplace và pDelete đều đen
				// pDelete là lá, sửa màu đen kép tại pDelete 
				fixDoubleBlack(root, pDelete);
			}
			else {
				// pReplace hoặc pDelete là đỏ
				if (findSibling(pDelete) != NULL)
					// node anh chị em không rỗng, làm cho nó màu đỏ 
					findSibling(pDelete)->Color = RED;
			}

			// Xóa pDelete khỏi cây 
			if (isOnLeft(pDelete)) {
				pParent->Left = NULL;
			}
			else {
				pParent->Right = NULL;
			}
		}
		delete pDelete;
		return;
	}

	if (pDelete->Left == NULL || pDelete->Right == NULL) {
		// pDelete có 1 node con
		if (pDelete == root) {
			// pDelete là gốc, gán giá trị của pReplace cho pDelete và xóa pReplace 
			pDelete->Data = pReplace->Data;
			pDelete->Left = pDelete->Right = NULL;
			delete pReplace;
		}
		else {
			// Tách node pDelete khỏi cây và di chuyển node pReplace lên
			if (isOnLeft(pDelete)) {
				pParent->Left = pReplace;
			}
			else {
				pParent->Right = pReplace;
			}
			delete pDelete;
			pReplace->Parent = pParent;
			if (flagDoubleBlack) {
				// pReplace và pDelete đều đen, sửa hai màu đen ở pReplace 
				fixDoubleBlack(root, pReplace);
			}
			else {
				// pReplace hoặc pDelete đỏ, màu pReplace đen 
				pReplace->Color = BLACK;
			}
		}
		return;
	}

	// pDelete có 2 con, hoán đổi giá trị với nút kế nhiệm (thế mạng) và đệ quy 
	swap(pReplace->Data, pDelete->Data);
	deleteNode(root, pReplace);
}
int countFactionLevelK(Tree root,int k)
{
	if (root)
	{
		k--;
		int a = countFactionLevelK(root->Left, k);
		int b = countFactionLevelK(root->Right, k);
		if (k == 0)
			return 1 + a + b;
		return a + b;
}
	return 0;
}
void deleteTree(Tree& root)
{
	if (!root) return;
	deleteTree(root->Left);
	deleteTree(root->Right);
	delete root;
}
int timUCLN(int a, int b)
{
	a = abs(a);
	b = abs(b);
	if (a == 0 || b == 0) return 1;
	while (a != b)
	{
		if (a > b) a -= b;
		else b -= a;
	}
	return a; //Hoặc b
}
void rutGonPhanSo(ItemType& p)
{
	int ucln = timUCLN(p.tu, p.mau);
	p.tu /= ucln;
	p.mau /= ucln;
}
ItemType congPhanSo(ItemType p1, ItemType p2)
{
	ItemType kq = { 0, 1 };
	kq.tu = p1.tu * p2.mau + p1.mau * p2.tu;
	kq.mau = p1.mau * p2.mau;
	rutGonPhanSo(kq);
	return kq;
}
ItemType sumNode(Tree root)
{ //Dùng phép duyệt cây LRN
	if (!root) return{ 0, 1 }; //Trả về phân số 0/1
	ItemType sl = sumNode(root->Left);	//Tổng giá trị các nút trên cây con trái
	ItemType sr = sumNode(root->Right);	//Tổng giá trị các nút trên cây con phải
	return congPhanSo(root->Data, congPhanSo(sl, sr));	//Cộng thêm giá trị của nút gốc
}
void menu()
{
	cout << "\n1. Nhap mang";
	cout << "\n2. Duyet NLR";
	cout<<"\n3. Duyet cay chieu rong NLR";
	cout << "\n4. Duyet cay chieu sau NLR";
	cout << "\n5. Them phan so";
	cout << "\n6. Tim kiem phan so";
	cout << "\n7. Xoa phan so";
	cout << "\n8. Tinh tong phan so";
	cout << "\n9. Dem so luong phan so muc K";
	cout << "\n10.Xoa cay";
	cout << "\nChon chuc nang: ";

}
int main()
{
	Tree root;
	createTree(root);
	ItemType a[] = { {4,7},{12,5},{3,5},{14,18},{16,17} };
	ItemType psTemp[] = { 0,1 };
	int chucnang;
	do
	{
		menu();
		cin >> chucnang;
		switch (chucnang)
		{
		case 1:
		{
			int n = 5;
			createTree(root);
			for (int i = 0; i < n; i++)
			{
				addNode(root, a[i]);
				set_Color(14, 2);
				cout << "\n Sau khi them nut: ";
				NLR(root);
			}
			break;
		}
		case 2:
		{
			NLR(root); break;
		}
		case 3:
		{
			if (root == NULL)
				cout << "\n Cây rỗng";
			else
				NLR_BFS(root);
			break;
		}
		case 4:
		{
			if (root == NULL)
				cout << "\n Cây rỗng";
			else
				NLR_DFS(root);
			break;
		}
		case 5:
		{
			PhanSo a;
			cout << "Nhap phan so can them: ";
			cin >> a.tu >> a.mau;
			addNode(root, a);
			NLR(root);
			break;
		}
		case 6:
		{
			PhanSo a;
			cout << "Nhap phan so can tim: ";
			cin >> a.tu >> a.mau;
			Node* k = search(root, a);
			if (root == NULL)
			{
				cout << "Khong tim thay";
			}
			else
			{
				cout << "Tim thay nut";
			}
		}
		case 7:
		{
			PhanSo a;
			cout << "Nhap phan so can xoa: ";
			cin >> a.tu >> a.mau; 
			Node* p = search(root, a);
			if (p == NULL);
			else
			{
				deleteNode(root, p);
			}
			NLR(root);
				break;

		}
		case 8:
		{
			cout << "S= " << sumNode(root).tu << "/" << sumNode(root).mau; break;
		}
		case 9:
		{
			int k;
			cout << "\nNhap muc k: ";
			cin >> k;
			cout << "So luong phan tu: " << countFactionLevelK(root, k); break;
		}
		case 10:
		{
			deleteTree(root);
			if (root == NULL)
				cout << "\nDa xoa xong!";
			else
				cout << "\nCay chua xoa!"; break;
		}
		default:
			break;
		}
	} while (chucnang!=0);
	return 0;
}
