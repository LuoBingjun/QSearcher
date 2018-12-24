
#include "CharString.h"
#include "htmlHandler.h"

class DocLList
{
public:
	//�ĵ�����ڵ���
	struct Node
	{
		Node(htmlInfo* _html, htmlInfo::word& _word, Node* _prior = nullptr, Node* _next = nullptr) :html(_html), word(_word), prior(_prior), next(_next) {}

		htmlInfo* html;	//����ҳ��ȫ����Ϣ
		int Times;	//���ָùؼ��ʵĴ���
		htmlInfo::word& word;
		Node* next = nullptr;
		Node* prior = nullptr;

		Node* operator++() {
			if (this)
				return next;
			else
				return nullptr;
		}
	};

	//�������������
	DocLList();
	~DocLList();

	//��ӡ��������༭��ɾ���ĵ��Ȳ���
	void Add(htmlInfo * _html, htmlInfo::word& word);
	bool Search(htmlInfo* _html);
	bool Search(int ID);
	htmlInfo* Edit(int ID);
	bool Remove(htmlInfo* _html);
	bool Remove(int ID);

	int length() const {
		return _length;
	}

	Node* head() {
		return iter_head;
	}

private:
	Node * iter_head = nullptr;	// ��ͷָ��
	Node* iter_tail = nullptr;	// ��βָ��
	int _length = 0;			// ����ĳ���
	CharString Term;			// �ؼ���
	int occur = 0;				// �ؼ��ʵ��ܴ�Ƶ
};