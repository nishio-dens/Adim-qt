#include <string>
#include <algorithm>
#include <iostream>
#include "Adim.h"
#include "Random.h"

void Adim::initAdim(const string& keycodeFilename, const string& workbookFilename,
		const string& candidateNumberFilename, 
		size_t numberOfQuestion ,
		bool randomQuestion ,size_t firstQuestionNumber,
		int sizeOfCharacterCode) 
{
	complete_ = false;
	ReadKeycode(keycodeFilename);
	ReadWorkbook(workbookFilename);
	InitKeycodeTableCandidateNumber();
	SetKeycodeTableCandidateNumberFilename(candidateNumberFilename);
	ReadKeycodeTableCandidateNumber();
	numberOfQuestion_ = numberOfQuestion;
	randomQuestion_ = randomQuestion;
	firstQuestionNumber_ = firstQuestionNumber;

	SetQuestion( 0 ); //dummy
	SetSizeOfCharacterCode(sizeOfCharacterCode);
}

void Adim::seedQuestion()
{
	numberOfQuestionArray_.clear();
	do{
		if( randomQuestion_ ) {
			for(int i=0; i < GetNumberOfWorkbookElement() ; ++i ) {
				numberOfQuestionArray_.push_back(i);
			}
			Random r;
			random_shuffle( numberOfQuestionArray_.begin(),
				numberOfQuestionArray_.end(), r );		
			std::cerr << "%%% call seed1. %%% \n";
		}else {
			for(int i=firstQuestionNumber_; i < GetNumberOfWorkbookElement() ; ++i ) {
				numberOfQuestionArray_.push_back(i);
			}
			for(int i=0; i < GetNumberOfWorkbookElement() ; ++i ) {
				numberOfQuestionArray_.push_back(i);
			}
			std::cerr << "%%% call seed2. %%% \n";
		}
	}while( numberOfQuestionArray_.size() < GetNumberOfWorkbookElement() );
}

//[返却値]
//2 成功 問題文打ち終わった COMPLETE
//1 成功 このまま続行 SUCCESS
//0 失敗 FAILURE
int Adim::InputCharacter(char code)
{
	int result = InputTypewordFromKeyboard(code);

	if( result == COMPLETE ) { //問題文を打ち終わった
		nowNumberOfQuestion_++;
		int num = nowNumberOfQuestion_;
		
		if( num >= GetNumberOfWorkbookElement() )
			num = 0;
		SetQuestion( numberOfQuestionArray_[num] );
		SaveKeycodeTableCandidateNumber(); //キーコードテーブルの保存
		if( nowNumberOfQuestion_ >= numberOfQuestion_ ){
			complete_ = true;
		}
	}
	return result;
}

string Adim::GetNowSentenceOfInputtedAgateQuestionRed() const
{
	size_t tmpPlus = GetNextAgateCaharcter().size();
	if( GetOneCharCompleteFlag() ) {
			return GetNowSentenceOfAgateQuestion().substr(0,
				GetNowPositionOfQuestion() + tmpPlus );
	}
	return GetNowSentenceOfAgateQuestion().substr(0,
		GetNowPositionOfQuestion() );
}

string Adim::GetNowSentenceOfInputtedConvertQuestionRed() const
{
	return GetNowSentenceOfConvertQuestion().substr(0,
		GetCompleteInputtedRoman().size() + 1 );
}

string Adim::GetNowSentenceOfInputtedAgateQuestionBlue() const
{
	size_t tmpPlus = GetNextAgateCaharcter().size();
	if( !GetOneCharCompleteFlag() ) {
		return GetNowSentenceOfAgateQuestion().substr(0,
			GetNowPositionOfQuestion() - tmpPlus );
	}
	return GetNowSentenceOfAgateQuestion().substr(0,GetNowPositionOfQuestion() );
}
string Adim::GetNowSentenceOfInputtedConvertQuestionBlue() const
{
	return GetCompleteInputtedRoman();
}

//temporary
//int main()
//{
//	using std::cout;
//	using std::endl;
//
//	Adim hoge;
//	try {
//		hoge.TempDebugFunction();
//	}catch(const exception& e) {
//		cout << e.what() << endl;
//	}
//	return 0;
//}
