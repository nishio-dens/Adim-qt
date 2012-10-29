#pragma once

#include <string>
#include <deque>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;
using std::deque;

//問題点
//wcout.imbue(locale("Japanese")); などうまく動いてくれない

//TODO: DebugまたはTrace関数の追加

//#define ON_LINUX_SYSTEM_

#ifdef ON_LINUX_SYSTEM_
	const int characterCodeLength_ = 3;
#else
	const int characterCodeLength_ = 2;
#endif

typedef struct WorkbookElement_tag
{
	string question;       //問題文
	string agateQuestion;  //ひらがな問題文
} WorkbookElement;

class AdimBase{
private:
	enum{ MAX_NUMBER_OF_QUESTION = 1000 };        //問題文の数の最大値
	map<string, deque<string> > keycodeTable;     //キーコードテーブル
	map<string, int> keycodeTableCandidateNumber; //ひらがな→ローマ字へ変換の際の文字変換候補
	string keycodeFilename;						  //キーコードテーブルのファイル名
	string KeycodeTableCandidateNumberFilename;   //文字変換候補のファイル名

	string workbookFilename;						  //問題文のファイル名
	WorkbookElement workbook[MAX_NUMBER_OF_QUESTION]; //問題文が収納されている構造体
	size_t numberOfWorkbookElement;					  //問題文の数
	
	string nowSentenceOfQuestion;			//現在入力予定の問題文
	string nowSentenceOfAgateQuestion;		//現在入力予定の問題文のひらがな版
	string nowSentenceOfConvertQuestion;	//現在入力予定の問題文のローマ字
	size_t    nowNumberOfQuestion;			//現在入力中の問題文の番号
	
	//ここからした1文字ないし2文字入力終了時初期化せよ
	deque<string> keycodeCandidate;		//現在入力中のひらがな1文字ないし2文字のの入力可能な候補
	string nowAgateCharacter;			//現在入力中のひらがな1文字ないし2文字
	string nextAgateCharacter;			//次にうつ文字
	size_t  nowPositionOfQuestion;		//AgateQuestion 何文字目まで入力されたか
	string inputtedRoman;				//入力されたローマ字日本語1文字または2文字用
	string completeInputtedRoman;		//いままで入力されたローマ字
	bool specialNnFlag;					//「ん」用のフラグ
	bool oneCharCompleteFlag;			//日本語一文字入力しおわったかの判定

	int sizeOfCharacterCode;			//文字コード 日本語一文字が何バイトか
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

