#pragma once

#include <string>
#include <deque>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;
using std::deque;

//���_
//wcout.imbue(locale("Japanese")); �Ȃǂ��܂������Ă���Ȃ�

//TODO: Debug�܂���Trace�֐��̒ǉ�

//#define ON_LINUX_SYSTEM_

#ifdef ON_LINUX_SYSTEM_
	const int characterCodeLength_ = 3;
#else
	const int characterCodeLength_ = 2;
#endif

typedef struct WorkbookElement_tag
{
	string question;       //��蕶
	string agateQuestion;  //�Ђ炪�Ȗ�蕶
} WorkbookElement;

class AdimBase{
private:
	enum{ MAX_NUMBER_OF_QUESTION = 1000 };        //��蕶�̐��̍ő�l
	map<string, deque<string> > keycodeTable;     //�L�[�R�[�h�e�[�u��
	map<string, int> keycodeTableCandidateNumber; //�Ђ炪�ȁ����[�}���֕ϊ��̍ۂ̕����ϊ����
	string keycodeFilename;						  //�L�[�R�[�h�e�[�u���̃t�@�C����
	string KeycodeTableCandidateNumberFilename;   //�����ϊ����̃t�@�C����

	string workbookFilename;						  //��蕶�̃t�@�C����
	WorkbookElement workbook[MAX_NUMBER_OF_QUESTION]; //��蕶�����[����Ă���\����
	size_t numberOfWorkbookElement;					  //��蕶�̐�
	
	string nowSentenceOfQuestion;			//���ݓ��͗\��̖�蕶
	string nowSentenceOfAgateQuestion;		//���ݓ��͗\��̖�蕶�̂Ђ炪�Ȕ�
	string nowSentenceOfConvertQuestion;	//���ݓ��͗\��̖�蕶�̃��[�}��
	size_t    nowNumberOfQuestion;			//���ݓ��͒��̖�蕶�̔ԍ�
	
	//�������炵��1�����Ȃ���2�������͏I��������������
	deque<string> keycodeCandidate;		//���ݓ��͒��̂Ђ炪��1�����Ȃ���2�����̂̓��͉\�Ȍ��
	string nowAgateCharacter;			//���ݓ��͒��̂Ђ炪��1�����Ȃ���2����
	string nextAgateCharacter;			//���ɂ�����
	size_t  nowPositionOfQuestion;		//AgateQuestion �������ڂ܂œ��͂��ꂽ��
	string inputtedRoman;				//���͂��ꂽ���[�}�����{��1�����܂���2�����p
	string completeInputtedRoman;		//���܂܂œ��͂��ꂽ���[�}��
	bool specialNnFlag;					//�u��v�p�̃t���O
	bool oneCharCompleteFlag;			//���{��ꕶ�����͂�����������̔���

	int sizeOfCharacterCode;			//�����R�[�h ���{��ꕶ�������o�C�g��
							//windows --> 2,  linuxFedora --> 3
protected:
	size_t GetNowNumberOfQuestion() const { return nowNumberOfQuestion; }
	size_t GetNowPositionOfQuestion() const { return nowPositionOfQuestion; }
	size_t GetNowCompletePositionOfQuestion() const
	{ return nowPositionOfQuestion - nowAgateCharacter.size(); }
	string GetCompleteInputtedRoman() const { return completeInputtedRoman; }
	string GetInputtedRoman() const { return inputtedRoman; }
	string GetNowAgateCharacter() const { return nowAgateCharacter; }
	string GetNextAgateCaharcter() const { return nextAgateCharacter; }
	bool   GetOneCharCompleteFlag() const { return oneCharCompleteFlag; }

	deque<string> SplitString(const string& str);

	void InitKeycodeTableCandidateNumber();
	bool SaveKeycodeTableCandidateNumber();
	bool ReadKeycodeTableCandidateNumber();
	
	bool IsDBCS (char check_byte) const;
	bool IsJapaneseVowel(const string& japaneseWord) const;
	bool IsJapaneseNaLine(const string& japaneseWord) const;
	bool IsJapaneseNN(const string& japaneseWord) const;
	bool IsJapaneseSmallLetter(const string& japaneseWord) const;
	bool IsJapaneseLtsu(const string& japaneseWord) const;
	bool IsVowel(const string& word) const;
	bool IsNN(const string& word) const;

	void SetSizeOfCharacterCode(int size=2 ) { sizeOfCharacterCode = size; }

    string ConvertQuestionToRoman(const string& question , size_t qPos = 0);
	deque<string> GetCandidate(const string& question, size_t& qPos, string& oneChar);
	deque<string> GetKeycode(const string& japaneseWord);

	size_t GetNumberOfWorkbookElement() const { return numberOfWorkbookElement; }

	void SetKeycodeTableCandidateNumberFilename(const string& filename)
	{ KeycodeTableCandidateNumberFilename = filename; }

	bool SetQuestion(size_t questionNumber);
	int InputTypewordFromKeyboard(char code);
	void ReadKeycode(const string& filename);
	void ReadWorkbook(const string& filename);
	void Trace(const string& message);

	enum{ FAILURE = 0, SUCCESS = 1, COMPLETE = 2 };

public:
	AdimBase() : keycodeFilename(""),
	nowSentenceOfQuestion(""),
	nowSentenceOfAgateQuestion(""),
	nowSentenceOfConvertQuestion(""),
    	nowNumberOfQuestion(0),
	KeycodeTableCandidateNumberFilename(""),
	sizeOfCharacterCode(characterCodeLength_) {}

	void SetKeycodeFilename(const string& filename) 
	{ keycodeFilename = filename; }
	void SetWorkbookFilename(const string& filename) 
	{ workbookFilename = filename; }
	const string& GetKeycodeFilename() const { return keycodeFilename; }
	const string& GetWorkbookFilename() const { return workbookFilename; }

	string GetNowSentenceOfQuestion() const { return nowSentenceOfQuestion; }
	string GetNowSentenceOfAgateQuestion() const { return nowSentenceOfAgateQuestion; }
	string GetNowSentenceOfConvertQuestion() const { return nowSentenceOfConvertQuestion; }

	void TempDebugFunction();
};

