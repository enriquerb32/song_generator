/***********************************************************************************************************************/                                                                            					   			 							         		
/* Purpose: The program allows you to create songs by generating random sentences, the words of which will be associated with a */
/* 			feeling, from the sentences of a text file. Then allow the user to save the songs in */
/* 			a new text file, identified by its title and average feeling. The user will have two forms of */
/* 			creation: the gention of as many sentences as you want one by one or the direct creation of complete songs */
/* 			from words with the same feeling. */
/***********************************************************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <time.h>

#define NPF 100			//Constant that defines the maximum number of words that can be stored in each sentence.
#define FTAM 500		//Constant that defines the maximum amount of sentences that the program can store.
#define STAM 3500		//Constant that defines the maximum number of words that can be saved.

using namespace std;

struct sentence	//It contains the whole sentence, the words separated one by one, and the total number of words in the sentence.
{
  	string sentence;		
  	string word[NPF];
  	int num;
};

struct word	//It contains the word and the feeling of it.
{
  	string word;
  	int valor;
};

void open_file (ifstream & f);
void read (int & n, int i);
int aleat (int rang);
void initialize (sentence & nova, int n);
int	select_sentences (int t, int i, int aux[FTAM]);
void add_words (string par, sentence & nova, int j);
int searchpos(word feeling[STAM], sentence & fgen, int & p);
void erase(sentence song[FTAM], int z);
void sentences_words (ifstream & f, sentence file[FTAM], int & i);
void feelings (sentence file[FTAM], word feeling[STAM], int i);
void gen_sentence (sentence file[FTAM], int i, sentence & fgen);
void sentence_feeling(word feeling[STAM], sentence & fgen);
void mark_sentence(sentence fgen, sentence song [FTAM], int & z, bool & guardar);
void write_song(sentence song[FTAM], ofstream & h, int z, float & sent_m, word feeling[STAM], char a);
void feel_song(sentence file[FTAM], word feeling[STAM], float & sent_m, sentence song[FTAM], int par, ofstream & h, char & a);


/*
 *Funció principal.
 */
int main(void) 
{
	ifstream f, g;
	ofstream h;
	string nom;
	sentence file[FTAM], fgen, song[FTAM];
	word feeling[STAM];
	int i, z=0, opcio;
	float sent_m;
	char tria, tria2, tria3, a;
	bool guardar;
    
  	srand(time(NULL));
  	
	open_file (f);
	
  	sentences_words (f, file, i);
  	feelings (file, feeling, i); 
  	do
	{
		cout << "Menu:" << endl;
		cout <<"1. Generate random sentences.\n2. Generate song according to the feeling.\n3. Exit.\n";
		cin >> opcio;
  		switch (opcio)
 		{
  			case 1:
  				
  				guardar = false;
  				a = 'l';
  				while (tria != 'n' && tria3 != 'n')
  				{
		  			gen_sentence (file, i, fgen);
				  
					sentence_feeling (feeling, fgen);	
			  	  
				    mark_sentence(fgen, song, z, guardar);
				  
				    cout << "Do you want another sentence? (y/n): " << endl;
				    cin >> tria;
				    if(tria=='n')
				    {
					    cout << "Do you want to erase everything? (y/n): " << endl;;
				  	    cin >> tria2;
				  	    if (tria2 == 's')
				  	    {
					  		erase (song, z);
					  		cout << "Do you want to start the song from scratch? (y/n): " << endl;
					    	cin >> tria3;
					    	guardar = false;
			  			}
		  			}
		  			cout << "\n";
		  		}
		  		
		  		if (tria3 != 'n' && guardar == true) //If no phrases are saved, boolean is not allowed to create any files for the song.
		  		{
			  		write_song(song, h, z, sent_m, feeling, a);	  			
				}
				
				tria = ' ';
				tria2 = ' ';
				tria3 = ' ';
				
	  			break;
  			case 2:
  				feel_song(file, feeling, sent_m, song, i, h, a);
  				break;
  			case 3:
  				break;
  			default:
  				cout << "Not a valid action.\n";
  				break;
  	
  		}
	}
	while(opcio!=3);
  
  	system ("Pause");
  	return 0;
}

/*
 * This function prompts for the filename
 * until it is able to open it
 */
void open_file (ifstream & f)
{
	string nom;
  		
  	do
  	{
	  	cout << "Enter the file name: " << endl;
	 	cin >> nom;
	  
	  	f.open(nom.c_str());
	  
	  	if (!f)
	  	{
	  		cout << "Error trying to open file." << "\n" << "\n";
	  	}
	}
	while (!f);		
}

/*
 * Procedure that asks for the number of words for the verse
 * until a valid value is entered
 */
void read (int & n, int i)
{
	do
	{
		n = 0;
		cout << "Enter the number of words you want for the verse: " << endl;
		cin >> n;		
	}
	while (n > i || n <= 0); // The value must be between 1 and the total number of sentences.	
}

/*
 * A function that generates and returns a random number in a given range.
 */
int aleat (int rang)
{
	int x;

	x = rand() * rang / RAND_MAX;
	return x;
}

/*
 * Procedure that empties the different fields of a phrase-type element.
 */
void initialize (sentence & nova, int n)
{
	int k;	// Iteration variable.
	
	nova.sentence = "";  	
	// n is the number of words in the sentence.
	for (k = 0; k <= n - 1; k++) 
	{
		nova.word[k] = "";
	}
	
	nova.num = 0;
}

/*
 * A function that receives a vector of integers each associated with a sentence in the file.
 * Generates random numbers until it finds one different from all the ones in the vector.
 * Returns that number as the identifier of a new sentence in the file.
 */
int	select_sentences (int t, int i, int aux[FTAM])
{
	int z;	// Iteration variable.
	int x;	// Number associated with the phrase in the file.
						
    do
	{
		z = 0;
		x = aleat (t);
		while (x != aux[z] && z < i)
		{
			z++	;
		} // Checks if the number has already been chosen before during the creation of the same generated phrase.
	}
	while (z != i); // It does not exit the loop until it finds a number different from those previously chosen during the creation of the same generated sentence.
						
	return x;
}

/*
 * Procedure that receives a word and a sentence and checks if the word is in the sentence.
 * If it's not there, it adds it to the sentence.
 */
void add_words (string par, sentence & nova, int j)
{
	int k = 0;
					
	while (nova.word[k] != par && k < nova.num)
	{
		k++;
	}	// Check if the word is already in the sentence.
					
	if (k == nova.num)
	{
		nova.sentence += par + " " ;	// Adds it to the tail of the sentence.
		nova.word[j] = par;		// Adds it to the phrase's word vector.		
	}	
}

/*
 * Find the position of the word within the vector 'feeling'.
 */
int searchpos(word feeling[STAM], sentence & fgen, int & p) 
{
	int j, trobat;
	
	j = 0;
	trobat = -1;
	for(j = 0; j < STAM && feeling[j-1].word != fgen.word[p]; j++)
	{
		trobat = j;
	}
	return trobat;
}
						
/*
 * Procedure that receives a vector of sentences and their size
 * and empties all fields of each element of the vector.
 */
void erase(sentence song[FTAM], int z)
{
	int l;
	
	for (l = 0; l < z; l++)
	{
		initialize (song[l], song[l].num);	// It sends to "initialize" the sentences in the vector one at a time.
	}
}

/*
 *It separates sentences and words.
 */
void sentences_words (ifstream & f, sentence file[FTAM], int & i) 
{
	int a = 0;
  	string text, sentence, word;
	
	i = 0;
  	text = f.get();
 	sentence = text;
  	word = text;
	  
  	while (!f.eof()) //It reads character by character until the end of the file.
  	{
		if (text == "." || text == "?" || text == "!") 
		{
	  		file[i].sentence = sentence; //When it reaches a point, it saves the phrase in the 'phrase' field.
	  		file[i].word[a] = word; //Save the word content in the 'word' field.
	  		file[i].num = a; //It tells us the number of words for each sentence.
	  
	  		i = i + 1;
	  		a = 0;
	  		text=f.get();
	  
	  		while (text == " " || text == "." || text == "\n") //Enhancement that avoids starting the next sentence with a space, and removes ellipses and line bots.
	    		text=f.get();
	    
	  		sentence = text;
	  
	  		if (text == ";" || text == ":" || text == "," || text == "\"" || text == "(" || text == ")") //Enhancement that prevents unwanted characters from being written to words.
	    		word="";                                          
      		else
        		word = text;
    	}
    
    	if (text == " ")
    	{
      		file[i].word[a] = word; //When it reaches a space, it stores the word information in the vector field 'word'.
      
      		a = a + 1;
      		text = f.get();
      
      		while (text == "\n" || text == " ") //Enhancement that avoids reading line bots and spaces.
	    		text = f.get();
      
      		sentence += text;
      
      		if (text == ";" || text == ":" || text == "," || text == "\"" || text == "(" || text == ")") //Enhancement that prevents unwanted characters from being written to words. 
	    		word = "";
      		else
        		word = text;
		}
		else
		{
      		text = f.get();
      
      		if (text != "\n") //Enhancement that avoids reading line breaks.
	  		{
	    		sentence += text;
        		if (text != " " && text != ";" && text != ":" && text != "," && text != "." && text != "\"" && text != ")" && text != "(") //Enhancement that prevents unwanted characters from being written to words. 
          			word += text;
     		}
    	}
  	}
  		
  	file[i].sentence = sentence; //In case the text does not end in a period, save the remaining content in the 'sentence' field.
	file[i].word[a] = word; //Saves the word content in the 'word' field.
	file[i].num = a; //Saves the number of words in the remaining sentence.
	i=i+1;
  
  	f.close();
}

/*
 * Groups repeated words from the 'sentence' record
 * as just one and assigns them feeling=0.
 */
void feelings (sentence file[FTAM], word feeling[STAM], int i) 
{
  	int a,b,c,d;
  	string word;
  
  	for (a = 0; a < i; a++) 
  	{
  		for (b = 0; b <= file[a].num; b++)
  		{
  	  		feeling[c].word = file[a].word[b]; //Passes all the words in the file to the 'word' field of the 'feeling' record.
  	  		c = c + 1;
		}
   	}
  
  	for (a = 0; a < c; a++)
  	{
		for (b = 0; b < a; b++)
    	{
	  		if (feeling[a].word == feeling[b].word && a!=b) //Removes repeated words from the string.
      		{
	    		for (d = a; d < (c-1); d++)
          			feeling[d].word = feeling[d+1].word;
          			
  				feeling[c].word = "";
  				c = c - 1;
  				a = a - 1;
      		}
    	}
  	}
  	
  	for (a = 0; a < c; a++) //Assigns feeling 0 to the words.
  		feeling[a].valor = 0;
}


/*
 * Procedure responsible for generating random sentences
 * with the length entered by the user.
 * The program selects a sentence from the file and a word
 * within this one as many times as there are words in the sentence.
 */

void gen_sentence (sentence file[FTAM], int i, sentence & fgen) 
{
	int h;				// Iteration variable. Represents the position of the word in the generated sentence.
	int n_par, x, y;	// Variables that represent the number of words in the generated sentence, the sentence chosen from the file and the word chosen from it.
	int aux[FTAM];  	// Vector of integers to avoid choosing words from the same sentence when creating a generated sentence.
	
	read (n_par, i);	

	initialize (fgen, n_par);
	fgen.num = n_par;

	// Loop for the creation of the sentence.
	for (h = 0; h < n_par; h++)	
	{
		x = select_sentences (i, h, aux);
	
		//Loop for choosing the new word.
		do 		
		{
			y = aleat (file[x].num);	
			
			add_words (file[x].word[y], fgen, h); 
		}
		while (fgen.word[h] == "");	// Carries out the operation until you have added the corresponding word.
		
		aux[h] = x;		// Saves the sentence number of the file used to avoid repetitions.
	}

	cout << fgen.sentence << "\n" << "\n";
}

/*
 * For each word of the generated sentence it assigns the feeling according to the user's response.
 * This feeling is assigned to the word of the vector 'feeling'.
 */

void sentence_feeling(word feeling[STAM], sentence & fgen) 	//It is executed after generating each sentence.
{
	int pos, p;
	char s;
	
	do
	{
		cout << "Write p if the sentence seems positive and n if it seems negative: " << "\n" << "\n";
		cin >> s;
		
		if (s != 'p' && s != 'n')	
		  cout << "Invalid entered value" << "\n" << "\n";
	}
	while (s != 'p' && s != 'n');
	
	for(p = 0; p < fgen.num; p++)
	{
		pos = searchpos(feeling, fgen, p);   //For each word in the sentence, find its position in the vector.
		
		if(s == 'p')
		{
			feeling[pos].valor = feeling[pos].valor + 1;
		}
		else
		{
			feeling[pos].valor = feeling[pos].valor - 1;	
		}
	}
}

/*
 * This procedure asks if you want to save the sentence and saves the
 * ones you want to keep
 */
void mark_sentence(sentence fgen, sentence song [FTAM], int & z, bool & guardar)  //'z' is the position of the sentence in the 'song' vector
{                                                         
	char op;  
	int k;  //iteration variable

	initialize (song[z], fgen.num);
	
	do
	{
		cout << "Do you want to save the sentence?  (y/n)" << endl;
		cin >> op;
		
		if(op != 's' && op != 'n')
		  cout << "Invalid action." << endl;
	}
	while(op != 's' && op != 'n');
		
	switch(op)
	{
		case 's':
			song[z].sentence = fgen.sentence;
			for (k = 0; k <= fgen.num - 1; k++)
				song[z].word[k] = fgen.word[k];
			
			song[z].num = fgen.num;
			z++;
			
			guardar=true;
			break;
		case 'n':  //in the case 'n' nothing should be done
			break;
	}
}

/*
 * This procedure writes the song in the file according to the feeling
 * and calculates the average feeling
 */
void write_song(sentence song[FTAM], ofstream & h, int z, float & sent_m, word feeling[STAM], char a) //the h file will be where we write the song
{                                                                                                            
	string titol;
	int p, k, l = 0, par = 0;
	
	sent_m = 0;  //we initialize the average feeling
	switch (a)   //for the calculation of sent_m according to the type of song
	{
		case 'l':
			for (l = 0; l < z; l++)  //loop for the average feeling calculation
			{
				for (p = 0; p < song[l].num; p++)
				{
					k = searchpos(feeling, song[l], p);
					sent_m += feeling[k].valor;
				}
				par = song[l].num + par;
			}
			
			sent_m = sent_m / par;  
			break;
			
		case 'p':  //sent_m is always 1 in this case
			sent_m = 1;
			break;
			
		case 'n':  //sent_m is always -1 in this case
			sent_m = -1;
			break;
	}
	
	cout << "Give me the title of the song: ";  
	cin.ignore();
	getline(cin, titol);
	
	h.open (titol.c_str ());
	h << "<" << titol << ">" << "    |< average feeling: " << sent_m << " >|";  //we write the song according to the given structure
	h << "\n";                                                                  //in the instructions of the exercise
	h << "\n";
	for (l = 0; l < z; l++)
	{
		h << song[l].sentence << endl;
	}
}


/*
 * Procedure created for creation
 * of songs according to a feeling
 * determinated by the user.
 */
void feel_song(sentence file[FTAM], word feeling[STAM], float & sent_m, sentence song[FTAM], int par, ofstream & h, char & a)
{
	int i, j;				//Iteration variables to count verses and words.
	int l;					//The l is an iteration variable to go through all the saved words.
	int n_vers, n_par;		//Variables to store the number of verses and words per verse, respectively.
	int x, y;				//Variables to gente a word randomly.
	int opcio;				//Integer to choose whether the created song is to be saved or not.
	int aux[FTAM];			//Vector of integers to avoid choosing words from the same sentence when creating a verse.
	
	
	//It prompts the user for the number of verses.
	do
	{
		cout << "How many verses do you want the song to have?\n";
		cin >> n_vers;
	}
	while (n_vers >= FTAM || n_vers <= 0);
	
	//It asks the user for the desired feeling.
	do
	{
		cout << "Do you want to create a positive or negative song? Write p for positive and n for negative."<<endl;
		cin >> a;
		if (a != 'n' && a != 'p')
			cout << "Invalid action" << endl;
	}
	while (a != 'n' && a != 'p');
	
	//Chooses one path or another according to the desired feeling.
	switch (a)
	{
		case 'p':
		{
			//Loop for writing verses.
			for( i = 0 ; i < n_vers ; i++ )
			{
				//For each verse, ask for the number of words it will contain.
				do
				{
					cout << "How many words will the verse have? " << i+1 << "?\n";
					cin >> n_par;
				}
				while (n_par > par || n_par <= 0);
				
				//initialization of the verse
				song[i].sentence = "";
				song[i].num = n_par;
				
				//Loop to select words randomly.
				for( j = 0 ; j < n_par ; j++ )
				{
					x = select_sentences (par, j, aux);
	
					do
					{	
						do
						{
							y = aleat(file[x].num);
	
							l = 0;
							while( (l <= STAM) && (file[x].word[y] != feeling[l].word) )
							{
								l++;
							}//Find the word in the 'feelings' vector.
						}
						while(feeling[l].valor < 0); //Stops the loop when the random word has the desired feeling.
						
						add_words (feeling[l].word, song[i], j);
					}
					while (song[i].word[j] == "");							
					
					aux[j] = x;
				}
			}						
		}
		break;
		//Same loop as the previous one but that chooses words with negative feeling.
		case 'n':
			{
				for( i = 0 ; i < n_vers ; i++ )
				{
					do
					{
						cout << "How many words will the verse have?" << i+1 << "?\n";
						cin >> n_par;
					}
					while (n_par > par || n_par <= 0);
					
					song[i].sentence = "";
					song[i].num = n_par;
					
					for( j = 0 ; j < n_par ; j++ )
					{
						x = select_sentences (par, j, aux);
						
						do
						{
							do
							{
								y = aleat(file[x].num);
								
								l = 0;
								while( (l <= STAM) && (file[x].word[y] != feeling[l].word) )
								{
									l++;
								}
							}
							while(feeling[l].valor > 0); //Discards words with a positive feeling.
							
							add_words (feeling[l].word, song[i], j);
						}
						while (song[i].word[j] == "");
						
						aux[j] = x;
					}
				}
			}
			break;
		default:
			break;
	}
	
	for( i = 0 ; i < n_vers ; i++ )
	{
		cout << song[i].sentence << endl;
	}	//Loop that writes the song
	
	cout << "If you want to save the song press 1.\n";
	cin >> opcio;
	if (opcio == 1)
	{
		write_song(song, h, n_vers, sent_m, feeling, a);		
	}	//the user chooses whether to save the song and calls the function that saves it.
}


