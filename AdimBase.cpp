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
* [関数名] IsJapaneseSmallLetter
* [ 機能 ] 日本語の小文字かどうか判定する
*---------------------------------------------------*/

bool AdimBase::IsJapaneseSmallLetter(const string& japaneseWord) const
{
	enum{ NUMBER_OF_SMALL_LETTER = 11 };
	const string small_letter[NUMBER_OF_SMALL_LETTER] =
	{"ぁ","ぃ","ぅ","ぇ","ぉ","ヵ","ヶ","ゃ","ゅ","ょ","っ"};
	for(int i=0 ; i < NUMBER_OF_SMALL_LETTER ; ++i ) {
		if( japaneseWord == small_letter[i] ) {
			return true;
		}
	}
	return false;
}

/*---------------------------------------------------
* [関数名] IsJapaneseVowel
* [ 機能 ] 日本語の母音かどうか判定する
*---------------------------------------------------*/

bool AdimBase::IsJapaneseVowel(const string& japaneseWord) const
{
	enum{ NUMBER_OF_VOWEL = 5 };
	const string vowel[NUMBER_OF_VOWEL] = {"あ","い","う","え","お"};
	for(int i=0 ; i < NUMBER_OF_VOWEL ; ++i ) {
		if( japaneseWord == vowel[i] ) {
			return true;
		}
	}
	return false;
}

/*---------------------------------------------------
* [関数名] IsVowel
* [ 機能 ] 母音かどうかを判定する
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
* [関数名] IsJapaneseNaLine
* [ 機能 ] な行かどうかを判定する
*---------------------------------------------------*/

bool AdimBase::IsJapaneseNaLine(const string& japaneseWord) const
{
	enum{ NUMBER_OF_NA_LINE = 5 };
	const string na_line[NUMBER_OF_NA_LINE] = {"な","に","ぬ","ね","の"};
	for(int i=0 ; i < NUMBER_OF_NA_LINE ; ++i ) {
		if( japaneseWord == na_line[i] ) {
			return true;
		}
	}
	return false;
}

/*---------------------------------------------------
* [関数名] IsJapaneseNN
* [ 機能 ] 「ん」かどうかを判定する
*---------------------------------------------------*/

bool AdimBase::IsJapaneseNN(const string& japaneseWord) const
{
	const string nn = "ん";
	if( japaneseWord == nn ) {
		return true;
	}
	return false;
}

/*---------------------------------------------------
* [関数名] IsNN
* [ 機能 ] n かどうかを判定する
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
* [関数名] IsJapaneseLtsu
* [ 機能 ] 「っ」かどうかを判定する
*---------------------------------------------------*/

bool AdimBase::IsJapaneseLtsu(const string& japaneseWord) const
{
	const string ltsu = "っ";
	if( japaneseWord == ltsu ) {
		return true;
	}
	return false;
}

/*---------------------------------------------------
* [関数名] IsDBCS
* [ 機能 ] DBCS文字かを判定する
*---------------------------------------------------*/

bool AdimBase::IsDBCS(char check_byte) const
{
	setlocale(LC_ALL, "Japanese");
	return ( isleadbyte( static_cast<unsigned char>(check_byte)) != 0 );
}

/*---------------------------------------------------
* [関数名] GetKeycode
* [ 機能 ] 日本語一文字無いし2文字をローマ字に変換してdequeに収納
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
* [関数名] SplitString
* [ 機能 ] 受け取った文章をスペースで区切り単語に直してdequeに収納する
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
* [関数名] ReadKeycode
* [ 機能 ] キーコードをファイルから読み込みKeycodeTableに収納する
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
* [関数名] ReadWorkbook
* [ 機能 ] 問題文をファイルから読み込みworkbookに収納する
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
* [関数名] SetQuestion
* [ 機能 ] 問題文のセットおよび使用変数の初期化
*---------------------------------------------------*/

bool AdimBase::SetQuestion(size_t questionNumber)
{
	if( questionNumber >= numberOfWorkbookElement ) //不正な問題番号
		return false;

	nowSentenceOfQuestion        = workbook[questionNumber].question;
	nowSentenceOfAgateQuestion   = workbook[questionNumber].agateQuestion;
	nowSentenceOfConvertQuestion = ConvertQuestionToRoman( nowSentenceOfAgateQuestion );
	nowNumberOfQuestion          = questionNumber;
	nowAgateCharacter			 = "";
	nowPositionOfQuestion        = 0; //問題文をはじめの位置から入力
	inputtedRoman				 = "";
	completeInputtedRoman		 = "";
	nextAgateCharacter			 = ""; //次に打つ日本語の初期化
	specialNnFlag				 = false;
	oneCharCompleteFlag			 = true;
	return true;
}

//[返却値]
//2 成功 問題文打ち終わった COMPLETE
//1 成功 このまま続行 SUCCESS
//0 失敗 FAILURE
int AdimBase::InputTypewordFromKeyboard(char code)
{
	if( specialNnFlag == true && code == 'n' ) { //ンを二回入力してもかまわない
		specialNnFlag = false;
		keycodeTableCandidateNumber["ん"] = 0;
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

	bool inputSuccess = false; //いま入力したローマ字が正しいかどうか
	size_t lengthRoman = inputtedRoman.size();

	size_t beforeCandidateNumber = 0;
	if( keycodeTableCandidateNumber.find(nowAgateCharacter) != keycodeTableCandidateNumber.end() ){
		beforeCandidateNumber = keycodeTableCandidateNumber[nowAgateCharacter];
	}
	
	vector<size_t> candidateNumber; //何番目と何番目が候補に挙がっているか
	//入力したローマ字が正しいかどうか判定
	for( deque<string>::size_type num = 0; num < keycodeCandidate.size(); ++num ) {
		string checkRoman = keycodeCandidate[num].substr( 0, lengthRoman );

		if( checkRoman == inputtedRoman ) { //途中まで入力成功の場合
			candidateNumber.push_back( num );
			inputSuccess = true;
		}
		if( inputtedRoman == keycodeCandidate[num] ) { //文字完全一致
			if( IsNN(inputtedRoman) ) { //ンをn一文字しか入力しなかった場合
				specialNnFlag = true;
			}
			oneCharCompleteFlag = true;
			candidateNumber.push_back( num );
		}
	}

	//候補を追加するかどうかの判定
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

	//いま入力した文字が正しくなかったらの処理
	if( !inputSuccess ) {
		inputtedRoman = beforeInputtedRoman;
	}else {
		completeInputtedRoman += code; //いままで入力したローマ字の追加
	}
	
	string ersAgateString = nowSentenceOfAgateQuestion.substr( GetNowCompletePositionOfQuestion() );
	nowSentenceOfConvertQuestion.erase( GetCompleteInputtedRoman().size() - GetInputtedRoman().size()
			, nowSentenceOfConvertQuestion.size() );
	nowSentenceOfConvertQuestion += ConvertQuestionToRoman( ersAgateString, 0 );

	if( oneCharCompleteFlag ) { //次に打つ文字を追加
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
* [関数名] Trace
* [ 機能 ] 
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
* [関数名] TempDebugFunction
* [ 機能 ] 
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
			SaveKeycodeTableCandidateNumber(); //キーコードテーブルの保存
		}
	}
	catch(const char* msg) {
		cout << msg << endl;
	}
}

/*---------------------------------------------------
* [関数名] GetCandidate
* [ 機能 ] 日本語1文字ないし2文字ないし3文字のローマ字の候補をdequeに追加
*---------------------------------------------------*/
//oneChar : いまから打つべき日本語ないし英語1文字を収納
//qPos    : 問題文何文字目？
//question: 問題文（ひらがな）

deque<string> AdimBase::GetCandidate(const string& question, size_t& qPos, string& oneChar)
{
	string convertedQuestion = ""; //実際に変換したローマ字（使わないかも）
	string strTmp;
	string subStrTmp;
	deque<string> candidate;
	deque<string> subCandidate;

	try{
		if( qPos < question.length() ) {
			if( IsDBCS(question[qPos]) ) {        //DBCS文字の処理
				strTmp = question.substr(qPos, sizeOfCharacterCode); //日本語一文字目
				candidate = GetKeycode(strTmp);   //ローマ字候補の追加

			//ッのときの処理 後で分割せよ
			//１文字目が「っ」の時は次の一文字を調べる必要がある
				if( IsJapaneseLtsu(strTmp) ) {
				
					string str_ltsu_tmp;
					string sub_str_ltsu_tmp;
					deque<string> ltsu_plus_candidate;
					deque<string> sub_ltsu_plus_candidate;

				//日本語2文字目をチェックする
					if( IsDBCS( question[qPos + sizeOfCharacterCode] ) ) {
					//ッの次の文字がDBCS文字の時
						str_ltsu_tmp = question.substr( qPos+sizeOfCharacterCode , sizeOfCharacterCode);   //日本語2文字目
						ltsu_plus_candidate = GetKeycode(str_ltsu_tmp);

					//ッの次の次の文字がDBCS文字だったら
					//例えば、「っきゃ」が存在したら候補に追加する
						if( IsDBCS( question[qPos + sizeOfCharacterCode*2] ) ) {
							sub_str_ltsu_tmp = question.substr( qPos +sizeOfCharacterCode, sizeOfCharacterCode*2);
							sub_ltsu_plus_candidate = GetKeycode(sub_str_ltsu_tmp);

							if( !sub_ltsu_plus_candidate.empty() ) {
								str_ltsu_tmp = sub_str_ltsu_tmp;
								ltsu_plus_candidate = sub_ltsu_plus_candidate;
							}
						}
					//ここからッの候補を追加する。
						string str_first_candidate;
						string str_candidate_tmp;

					//子音重ねの「っ」の候補を代入するかどうかの判定
						for( deque<string>::iterator pos = ltsu_plus_candidate.begin();
							pos != ltsu_plus_candidate.end() ; ++pos ) {
						str_candidate_tmp = (pos[0])[0]; //「っ」の次の文字の子音候補
						if( !IsVowel(str_candidate_tmp) &&
									!IsNN(str_candidate_tmp) &&
										find( candidate.begin(), candidate.end(), str_ltsu_tmp) == candidate.end() ) {
										candidate.push_back(str_candidate_tmp);
										}
										}
					} //ッ二文字目チェックここまで
				} //ッの処理ここまで
				else if( IsJapaneseNN(strTmp) ) {
				//ンのときの処理				
					if( IsDBCS( question[qPos + sizeOfCharacterCode] ) ) {
						string str_n_tmp;
						str_n_tmp = question.substr(qPos + sizeOfCharacterCode,sizeOfCharacterCode);
                    
						if( IsJapaneseNaLine(str_n_tmp) ||
										IsJapaneseNN(str_n_tmp)      ||
										IsJapaneseVowel(str_n_tmp)   ) {
							convertedQuestion += candidate.at(0);
							deque<string>::iterator eraseCandidate =
									find( candidate.begin(), candidate.end(), "n" ); //nだけの入力を削除
							candidate.erase( eraseCandidate );
							//convertedQuestion += candidate.at(0);
										}else{}
					}else { //文章の最後が「ん」で終わる時
						deque<string>::iterator eraseCandidate =
								find( candidate.begin(), candidate.end(), "n" );
						candidate.erase( eraseCandidate );
						convertedQuestion += candidate.at(0);
					
					}
				} //ンの処理ここまで
			//「きゃ」など日本語2文字の時の処理
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
			//候補をローマ文に変換
				try{
					if( keycodeTableCandidateNumber.find(strTmp) != keycodeTableCandidateNumber.end()) {
						convertedQuestion += candidate.at(
								keycodeTableCandidateNumber[strTmp]);
					}
				}catch(const out_of_range&) {
					convertedQuestion += candidate.at(0);
				}
			//日本語なのでPositionを２進める
				oneChar = strTmp;
				qPos += sizeOfCharacterCode;
			}
			else { //入力文字がDBCS文字でないときの処理
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
* [関数名] ConvertQuestionToRoma
* [ 機能 ] ひらがな文をローマ文に変換する
* [ 注意 ] qPos はデフォルト引数 = 0
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
				}else if( !IsDBCS(strTmp[0]) ) { //英文字だったらローマ字になってるのでそのまま追加
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
* [関数名] InitKeycodeTableCandidateNumber
* [ 機能 ] キーコード候補初期化
*---------------------------------------------------*/

void AdimBase::InitKeycodeTableCandidateNumber()
{
	map< string, deque<string> >::iterator pos;

	for( pos = keycodeTable.begin() ; pos != keycodeTable.end() ; ++pos ) {
		keycodeTableCandidateNumber[pos->first] = 0;
	}
}

/*---------------------------------------------------
* [関数名] SaveKeycodeTableCandidateNumber
* [ 機能 ] キーコード候補セーブ
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
* [関数名] ReadKeycodeTableCandidateNumber
* [ 機能 ] キーコード候補の読み込み
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


