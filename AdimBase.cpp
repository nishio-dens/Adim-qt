#include "AdimBase.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <exception>

#ifdef ON_LINUX_SYSTEM_

int isleadbyte(unsigned char check)
{
	if( ( (check >= 0x81) && (check <= 0x9F) ) || ( (check >= 0xE0) && ( check <= 0xFC ) ) )
		return 1;
	return 0;
}

#endif

using namespace std;

/*---------------------------------------------------
* [�֐���] IsJapaneseSmallLetter
* [ �@�\ ] ���{��̏��������ǂ������肷��
*---------------------------------------------------*/

bool AdimBase::IsJapaneseSmallLetter(const string& japaneseWord) const
{
	enum{ NUMBER_OF_SMALL_LETTER = 11 };
	const string small_letter[NUMBER_OF_SMALL_LETTER] =
	{"��","��","��","��","��","��","��","��","��","��","��"};
	for(int i=0 ; i < NUMBER_OF_SMALL_LETTER ; ++i ) {
		if( japaneseWord == small_letter[i] ) {
			return true;
		}
	}
	return false;
}

/*---------------------------------------------------
* [�֐���] IsJapaneseVowel
* [ �@�\ ] ���{��̕ꉹ���ǂ������肷��
*---------------------------------------------------*/

bool AdimBase::IsJapaneseVowel(const string& japaneseWord) const
{
	enum{ NUMBER_OF_VOWEL = 5 };
	const string vowel[NUMBER_OF_VOWEL] = {"��","��","��","��","��"};
	for(int i=0 ; i < NUMBER_OF_VOWEL ; ++i ) {
		if( japaneseWord == vowel[i] ) {
			return true;
		}
	}
	return false;
}

/*---------------------------------------------------
* [�֐���] IsVowel
* [ �@�\ ] �ꉹ���ǂ����𔻒肷��
*---------------------------------------------------*/

bool AdimBase::IsVowel(const string& word) const
{
	enum{ NUMBER_OF_VOWEL = 5 };
	const string vowel[NUMBER_OF_VOWEL] = {"a","i","u","e","o"};
	for(int i=0 ; i < NUMBER_OF_VOWEL ; ++i ) {
		if( word == vowel[i] ) {
			return true;
		}
	}
	return false;
}

/*---------------------------------------------------
* [�֐���] IsJapaneseNaLine
* [ �@�\ ] �ȍs���ǂ����𔻒肷��
*---------------------------------------------------*/

bool AdimBase::IsJapaneseNaLine(const string& japaneseWord) const
{
	enum{ NUMBER_OF_NA_LINE = 5 };
	const string na_line[NUMBER_OF_NA_LINE] = {"��","��","��","��","��"};
	for(int i=0 ; i < NUMBER_OF_NA_LINE ; ++i ) {
		if( japaneseWord == na_line[i] ) {
			return true;
		}
	}
	return false;
}

/*---------------------------------------------------
* [�֐���] IsJapaneseNN
* [ �@�\ ] �u��v���ǂ����𔻒肷��
*---------------------------------------------------*/

bool AdimBase::IsJapaneseNN(const string& japaneseWord) const
{
	const string nn = "��";
	if( japaneseWord == nn ) {
		return true;
	}
	return false;
}

/*---------------------------------------------------
* [�֐���] IsNN
* [ �@�\ ] n ���ǂ����𔻒肷��
*---------------------------------------------------*/

bool AdimBase::IsNN(const string& word) const
{
	const string nn = "n";
	if( word == nn ) {
		return true;
	}
	return false;
}

/*---------------------------------------------------
* [�֐���] IsJapaneseLtsu
* [ �@�\ ] �u���v���ǂ����𔻒肷��
*---------------------------------------------------*/

bool AdimBase::IsJapaneseLtsu(const string& japaneseWord) const
{
	const string ltsu = "��";
	if( japaneseWord == ltsu ) {
		return true;
	}
	return false;
}

/*---------------------------------------------------
* [�֐���] IsDBCS
* [ �@�\ ] DBCS�������𔻒肷��
*---------------------------------------------------*/

bool AdimBase::IsDBCS(char check_byte) const
{
	setlocale(LC_ALL, "Japanese");
	return ( isleadbyte( static_cast<unsigned char>(check_byte)) != 0 );
}

/*---------------------------------------------------
* [�֐���] GetKeycode
* [ �@�\ ] ���{��ꕶ��������2���������[�}���ɕϊ�����deque�Ɏ��[
*---------------------------------------------------*/

deque<string> AdimBase::GetKeycode(const string& japaneseWord)
{
	map<string, deque<string> >::iterator pos;

	pos = keycodeTable.find(japaneseWord);
	if( pos != keycodeTable.end() ) {
		return keycodeTable[japaneseWord];
	}
	deque<string> tmp;
	return tmp;
}

/*---------------------------------------------------
* [�֐���] SplitString
* [ �@�\ ] �󂯎�������͂��X�y�[�X�ŋ�؂�P��ɒ�����deque�Ɏ��[����
*---------------------------------------------------*/

deque<string> AdimBase::SplitString(const string& str)
{
	string::size_type i = 0 , j = 0;
	string::size_type max_len = str.length();
	deque<string> tmp_deque;

	while( i < max_len ) {
		while( i < max_len && isspace( (unsigned char)str[i] ) )
			++i;
		j = i;
		while( j < max_len && !isspace( (unsigned char)str[j] ) )
			++j;
		if( i != j ) {
			tmp_deque.push_back( str.substr(i , j-i) );
			i = j;
		}
	}
	return tmp_deque;
}

/*---------------------------------------------------
* [�֐���] ReadKeycode
* [ �@�\ ] �L�[�R�[�h���t�@�C������ǂݍ���KeycodeTable�Ɏ��[����
*---------------------------------------------------*/

void AdimBase::ReadKeycode(const string& filename)
{
	SetKeycodeFilename(filename);
	ifstream file( keycodeFilename.c_str() );
	deque<string> tmp_deque;
	string tmp_str , jp_word;

	try {
		if( !file ) {
			throw "Keycode File Open Error";
		}
		
		while( !file.eof() ) {
			getline(file , tmp_str);
			tmp_deque = SplitString( tmp_str );
			
			jp_word = tmp_deque[0];
			tmp_deque.pop_front();

			back_insert_iterator< deque<string> > iter( keycodeTable[ jp_word ] );
			copy( tmp_deque.begin() , tmp_deque.end() , iter );
		}
	} catch( const char* msg ) {
		cout << msg << endl;
		Trace(msg);
		throw;
	} catch (...) {
		terminate();
	}
}

/*---------------------------------------------------
* [�֐���] ReadWorkbook
* [ �@�\ ] ��蕶���t�@�C������ǂݍ���workbook�Ɏ��[����
*---------------------------------------------------*/

void AdimBase::ReadWorkbook(const string& filename)
{
	SetWorkbookFilename(filename);
	ifstream file( workbookFilename.c_str() );
	string tmp_str, tmp_agate_str;
	int i;

	try {
		if( !file ) {
			throw "Wordbook File Open Error";
		}

		for(i=0; !file.eof() ; ++i ) {
			getline(file , tmp_str);
			getline(file, tmp_agate_str);
			workbook[i].question = tmp_str;
			workbook[i].agateQuestion = tmp_agate_str;
		}
		numberOfWorkbookElement = i;
	} catch( const char* msg ) {
		cout << msg << endl;
		Trace(msg);
		throw;
	} catch (...) {
		terminate();
	}
}

/*---------------------------------------------------
* [�֐���] SetQuestion
* [ �@�\ ] ��蕶�̃Z�b�g����юg�p�ϐ��̏�����
*---------------------------------------------------*/

bool AdimBase::SetQuestion(size_t questionNumber)
{
	if( questionNumber >= numberOfWorkbookElement ) //�s���Ȗ��ԍ�
		return false;

	nowSentenceOfQuestion        = workbook[questionNumber].question;
	nowSentenceOfAgateQuestion   = workbook[questionNumber].agateQuestion;
	nowSentenceOfConvertQuestion = ConvertQuestionToRoman( nowSentenceOfAgateQuestion );
	nowNumberOfQuestion          = questionNumber;
	nowAgateCharacter			 = "";
	nowPositionOfQuestion        = 0; //��蕶���͂��߂̈ʒu�������
	inputtedRoman				 = "";
	completeInputtedRoman		 = "";
	nextAgateCharacter			 = ""; //���ɑł��{��̏�����
	specialNnFlag				 = false;
	oneCharCompleteFlag			 = true;
	return true;
}

//[�ԋp�l]
//2 ���� ��蕶�ł��I����� COMPLETE
//1 ���� ���̂܂ܑ��s SUCCESS
//0 ���s FAILURE
int AdimBase::InputTypewordFromKeyboard(char code)
{
	if( specialNnFlag == true && code == 'n' ) { //��������͂��Ă����܂�Ȃ�
		specialNnFlag = false;
		keycodeTableCandidateNumber["��"] = 0;
		completeInputtedRoman += 'n';
		return SUCCESS;
	}

	if( oneCharCompleteFlag ) {
		oneCharCompleteFlag = false;
		specialNnFlag = false;
		nowAgateCharacter = "";
		inputtedRoman = "";
		keycodeCandidate = GetCandidate( nowSentenceOfAgateQuestion, nowPositionOfQuestion, nowAgateCharacter );
	}
	
	string beforeInputtedRoman = inputtedRoman;
	inputtedRoman += code;

	bool inputSuccess = false; //���ܓ��͂������[�}�������������ǂ���
	size_t lengthRoman = inputtedRoman.size();

	size_t beforeCandidateNumber = 0;
	if( keycodeTableCandidateNumber.find(nowAgateCharacter) != keycodeTableCandidateNumber.end() ){
		beforeCandidateNumber = keycodeTableCandidateNumber[nowAgateCharacter];
	}
	
	vector<size_t> candidateNumber; //���ԖڂƉ��Ԗڂ����ɋ������Ă��邩
	//���͂������[�}�������������ǂ�������
	for( deque<string>::size_type num = 0; num < keycodeCandidate.size(); ++num ) {
		string checkRoman = keycodeCandidate[num].substr( 0, lengthRoman );

		if( checkRoman == inputtedRoman ) { //�r���܂œ��͐����̏ꍇ
			candidateNumber.push_back( num );
			inputSuccess = true;
		}
		if( inputtedRoman == keycodeCandidate[num] ) { //�������S��v
			if( IsNN(inputtedRoman) ) { //����n�ꕶ���������͂��Ȃ������ꍇ
				specialNnFlag = true;
			}
			oneCharCompleteFlag = true;
			candidateNumber.push_back( num );
		}
	}

	//����ǉ����邩�ǂ����̔���
	bool addCandidate = true;
	for( size_t count = 0; count < candidateNumber.size() ; count++ ) {
		if( candidateNumber[count] == beforeCandidateNumber ) {
			addCandidate = false;
			break;
		}
	}
	if( inputSuccess ) {
		if( addCandidate && candidateNumber.size() != 0 &&
			keycodeTableCandidateNumber.find(nowAgateCharacter) != keycodeTableCandidateNumber.end() ) {
			keycodeTableCandidateNumber[nowAgateCharacter] = candidateNumber[0];
				  } else if( addCandidate && candidateNumber.size() == 0 &&
					 keycodeTableCandidateNumber.find(nowAgateCharacter) != keycodeTableCandidateNumber.end()) {
						keycodeTableCandidateNumber[nowAgateCharacter] = 0;
					}
	}

	//���ܓ��͂����������������Ȃ�������̏���
	if( !inputSuccess ) {
		inputtedRoman = beforeInputtedRoman;
	}else {
		completeInputtedRoman += code; //���܂܂œ��͂������[�}���̒ǉ�
	}
	
	string ersAgateString = nowSentenceOfAgateQuestion.substr( GetNowCompletePositionOfQuestion() );
	nowSentenceOfConvertQuestion.erase( GetCompleteInputtedRoman().size() - GetInputtedRoman().size()
			, nowSentenceOfConvertQuestion.size() );
	nowSentenceOfConvertQuestion += ConvertQuestionToRoman( ersAgateString, 0 );

	if( oneCharCompleteFlag ) { //���ɑł�����ǉ�
		size_t tmpPos = nowPositionOfQuestion;
		GetCandidate( nowSentenceOfAgateQuestion, tmpPos , nextAgateCharacter );
	}
	if( nowSentenceOfAgateQuestion.substr(0,nowPositionOfQuestion) == nowSentenceOfAgateQuestion &&
		   oneCharCompleteFlag) {
		return COMPLETE;
		   }else if( inputSuccess ) {
			   return SUCCESS;
		   }
		   return FAILURE;
}

/*---------------------------------------------------
* [�֐���] Trace
* [ �@�\ ] 
*---------------------------------------------------*/
void AdimBase::Trace(const string& message)
{
	ofstream log("errorLog.txt");
	if( !log ) {
		return;
	}
	log << message << endl;
}


/*---------------------------------------------------
* [�֐���] TempDebugFunction
* [ �@�\ ] 
*---------------------------------------------------*/

void AdimBase::TempDebugFunction()
{
	unsigned int num = 0;

	ReadKeycode("keycode.txt");
	ReadWorkbook("word.txt");

	InitKeycodeTableCandidateNumber();
	SetKeycodeTableCandidateNumberFilename("Candidate_Number.txt");
	ReadKeycodeTableCandidateNumber();
	
	try{
		for(;num < numberOfWorkbookElement ; num++) {
			if ( !SetQuestion(num) )
				throw "Invalidate Number of question in TempDebugFunction";
			cout << "Number Of Question :" << nowNumberOfQuestion << endl;
			cout << nowSentenceOfQuestion << endl;
			cout << nowSentenceOfAgateQuestion << endl;
			cout << nowSentenceOfConvertQuestion << endl;
			int result;
			do{
				char oneChar;
				cin >> oneChar;
				result = InputTypewordFromKeyboard(oneChar);
				if(result==0) {
					cout << "TYPE ERROR!" << endl;
				}
				cout << "now :" << nowSentenceOfAgateQuestion.substr(0,GetNowCompletePositionOfQuestion() );
				cout << " cand:" << nowAgateCharacter << " pos:" << nowPositionOfQuestion;
				cout << " inp:" << inputtedRoman << " next:" << nextAgateCharacter <<endl;
				cout << "ques:" << nowSentenceOfConvertQuestion << endl;
				cout << "comp:" << GetCompleteInputtedRoman() << endl;
			}while(result != 2);
			SaveKeycodeTableCandidateNumber(); //�L�[�R�[�h�e�[�u���̕ۑ�
		}
	}
	catch(const char* msg) {
		cout << msg << endl;
	}
}

/*---------------------------------------------------
* [�֐���] GetCandidate
* [ �@�\ ] ���{��1�����Ȃ���2�����Ȃ���3�����̃��[�}���̌���deque�ɒǉ�
*---------------------------------------------------*/
//oneChar : ���܂���łׂ����{��Ȃ����p��1���������[
//qPos    : ��蕶�������ځH
//question: ��蕶�i�Ђ炪�ȁj

deque<string> AdimBase::GetCandidate(const string& question, size_t& qPos, string& oneChar)
{
	string convertedQuestion = ""; //���ۂɕϊ��������[�}���i�g��Ȃ������j
	string strTmp;
	string subStrTmp;
	deque<string> candidate;
	deque<string> subCandidate;

	try{
		if( qPos < question.length() ) {
			if( IsDBCS(question[qPos]) ) {        //DBCS�����̏���
				strTmp = question.substr(qPos, sizeOfCharacterCode); //���{��ꕶ����
				candidate = GetKeycode(strTmp);   //���[�}�����̒ǉ�

			//�b�̂Ƃ��̏��� ��ŕ�������
			//�P�����ڂ��u���v�̎��͎��̈ꕶ���𒲂ׂ�K�v������
				if( IsJapaneseLtsu(strTmp) ) {
				
					string str_ltsu_tmp;
					string sub_str_ltsu_tmp;
					deque<string> ltsu_plus_candidate;
					deque<string> sub_ltsu_plus_candidate;

				//���{��2�����ڂ��`�F�b�N����
					if( IsDBCS( question[qPos + sizeOfCharacterCode] ) ) {
					//�b�̎��̕�����DBCS�����̎�
						str_ltsu_tmp = question.substr( qPos+sizeOfCharacterCode , sizeOfCharacterCode);   //���{��2������
						ltsu_plus_candidate = GetKeycode(str_ltsu_tmp);

					//�b�̎��̎��̕�����DBCS������������
					//�Ⴆ�΁A�u������v�����݂�������ɒǉ�����
						if( IsDBCS( question[qPos + sizeOfCharacterCode*2] ) ) {
							sub_str_ltsu_tmp = question.substr( qPos +sizeOfCharacterCode, sizeOfCharacterCode*2);
							sub_ltsu_plus_candidate = GetKeycode(sub_str_ltsu_tmp);

							if( !sub_ltsu_plus_candidate.empty() ) {
								str_ltsu_tmp = sub_str_ltsu_tmp;
								ltsu_plus_candidate = sub_ltsu_plus_candidate;
							}
						}
					//��������b�̌���ǉ�����B
						string str_first_candidate;
						string str_candidate_tmp;

					//�q���d�˂́u���v�̌��������邩�ǂ����̔���
						for( deque<string>::iterator pos = ltsu_plus_candidate.begin();
							pos != ltsu_plus_candidate.end() ; ++pos ) {
						str_candidate_tmp = (pos[0])[0]; //�u���v�̎��̕����̎q�����
						if( !IsVowel(str_candidate_tmp) &&
									!IsNN(str_candidate_tmp) &&
										find( candidate.begin(), candidate.end(), str_ltsu_tmp) == candidate.end() ) {
										candidate.push_back(str_candidate_tmp);
										}
										}
					} //�b�񕶎��ڃ`�F�b�N�����܂�
				} //�b�̏��������܂�
				else if( IsJapaneseNN(strTmp) ) {
				//���̂Ƃ��̏���				
					if( IsDBCS( question[qPos + sizeOfCharacterCode] ) ) {
						string str_n_tmp;
						str_n_tmp = question.substr(qPos + sizeOfCharacterCode,sizeOfCharacterCode);
                    
						if( IsJapaneseNaLine(str_n_tmp) ||
										IsJapaneseNN(str_n_tmp)      ||
										IsJapaneseVowel(str_n_tmp)   ) {
							convertedQuestion += candidate.at(0);
							deque<string>::iterator eraseCandidate =
									find( candidate.begin(), candidate.end(), "n" ); //n�����̓��͂��폜
							candidate.erase( eraseCandidate );
							//convertedQuestion += candidate.at(0);
										}else{}
					}else { //���͂̍Ōオ�u��v�ŏI��鎞
						deque<string>::iterator eraseCandidate =
								find( candidate.begin(), candidate.end(), "n" );
						candidate.erase( eraseCandidate );
						convertedQuestion += candidate.at(0);
					
					}
				} //���̏��������܂�
			//�u����v�ȂǓ��{��2�����̎��̏���
				else if( IsDBCS( question[qPos + sizeOfCharacterCode] ) ) {
					subStrTmp = question.substr( qPos , sizeOfCharacterCode*2);
					subCandidate = GetKeycode(subStrTmp);
				
					if( !subCandidate.empty() ) {
						strTmp = subStrTmp;
						candidate = subCandidate;
						qPos+=2;
					//break;
					}
				}
			//�������[�}���ɕϊ�
				try{
					if( keycodeTableCandidateNumber.find(strTmp) != keycodeTableCandidateNumber.end()) {
						convertedQuestion += candidate.at(
								keycodeTableCandidateNumber[strTmp]);
					}
				}catch(const out_of_range&) {
					convertedQuestion += candidate.at(0);
				}
			//���{��Ȃ̂�Position���Q�i�߂�
				oneChar = strTmp;
				qPos += sizeOfCharacterCode;
			}
			else { //���͕�����DBCS�����łȂ��Ƃ��̏���
				convertedQuestion += question[qPos];
				string notDBCS = "";
				notDBCS += question[qPos];
				candidate.push_back( notDBCS );
				oneChar = notDBCS;
				++qPos;
			}
		}
	}catch(const exception& e) {
		cout << e.what() << " in Get Candidate function." << endl;
	}catch(...) {
		cout << "unknown error in Get Candidate function." << endl;
	}
	return candidate;
}

/*---------------------------------------------------
* [�֐���] ConvertQuestionToRoma
* [ �@�\ ] �Ђ炪�ȕ������[�}���ɕϊ�����
* [ ���� ] qPos �̓f�t�H���g���� = 0
*---------------------------------------------------*/

string AdimBase::ConvertQuestionToRoman(const string& question , size_t qPos)
{
	string convertedQuestion = "";
	string strTmp;
	deque<string> candidate;

	try{
		for( ;qPos < question.length(); strTmp = "" ) {
			candidate = GetCandidate(question , qPos , strTmp );

			try{
				if( keycodeTableCandidateNumber.find(strTmp) != keycodeTableCandidateNumber.end() ) {
					convertedQuestion += candidate.at( keycodeTableCandidateNumber[strTmp] );
				}else if( !IsDBCS(strTmp[0]) ) { //�p�����������烍�[�}���ɂȂ��Ă�̂ł��̂܂ܒǉ�
					convertedQuestion += strTmp;
				}
			}catch(const out_of_range&) {
				convertedQuestion += candidate.at(0);
			}
		}
	}catch(const exception& e) {
		cout << e.what() << " in convert question to Roman function." << endl;
	}catch(...) {
		cout << "unknown error in convert question to Roman function." << endl;
	}
	return convertedQuestion;
}

/*---------------------------------------------------
* [�֐���] InitKeycodeTableCandidateNumber
* [ �@�\ ] �L�[�R�[�h��⏉����
*---------------------------------------------------*/

void AdimBase::InitKeycodeTableCandidateNumber()
{
	map< string, deque<string> >::iterator pos;

	for( pos = keycodeTable.begin() ; pos != keycodeTable.end() ; ++pos ) {
		keycodeTableCandidateNumber[pos->first] = 0;
	}
}

/*---------------------------------------------------
* [�֐���] SaveKeycodeTableCandidateNumber
* [ �@�\ ] �L�[�R�[�h���Z�[�u
*---------------------------------------------------*/

bool AdimBase::SaveKeycodeTableCandidateNumber()
{
	ofstream file( KeycodeTableCandidateNumberFilename.c_str() );

	if( !file ) {
		cout << "Error Save Keycode Table Candidate Number function." << endl;
		return false;
	}

	map< string, int >::iterator pos;

	for( pos = keycodeTableCandidateNumber.begin() ;
		    pos != keycodeTableCandidateNumber.end() ; ++pos ) {
			    file << pos->first << " " << pos->second << endl;
		    }
		    return true;
}

/*---------------------------------------------------
* [�֐���] ReadKeycodeTableCandidateNumber
* [ �@�\ ] �L�[�R�[�h���̓ǂݍ���
*---------------------------------------------------*/

bool AdimBase::ReadKeycodeTableCandidateNumber()
{
	ifstream file( KeycodeTableCandidateNumberFilename.c_str() );

	if( !file ) {
		cout << "Erorr Read Keycode Table Candidate Number function." << endl;
		return false;
	}

	string str;
	int tmp;
	keycodeTableCandidateNumber.clear();
	while( !file.eof() ) {
		file >> str;
		file >> tmp;
		keycodeTableCandidateNumber[str] = tmp;
	}
	return true;
}


