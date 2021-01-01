#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>

using namespace std;

class Card{
public:
	Card(string col, string v){color = col; val = v;}

	string color;
	string val;
};


bool bTurn = 1;
string curColor;
vector<Card> deck;
vector<Card> discardPile;
vector<Card> playerHand;
vector<Card> CPUHand;


void initializeDeck(){
	for(int i=0;i<14;i++){
		string value = to_string(i);

		if(i == 10){
			value = "skip";
		}
		if(i==11){
			value = "reverse";
		}
		if(i==12){
			value = "takeTwo";
		}

		if(i==0){
			deck.push_back(Card("red", value));
			deck.push_back(Card("yellow", value));
			deck.push_back(Card("green", value));
			deck.push_back(Card("blue", value));
			continue;
		}

		if(i==13){
			for(int j = 0; j< 4; j++){
				deck.push_back(Card("wild", ""));
				deck.push_back(Card("wild", "takeFour"));
			}
			return;
		}
		
		deck.push_back(Card("red", value));
		deck.push_back(Card("red", value));
		deck.push_back(Card("yellow", value));
		deck.push_back(Card("yellow", value));
		deck.push_back(Card("green", value));
		deck.push_back(Card("green", value));
		deck.push_back(Card("blue", value));
		deck.push_back(Card("blue", value));
	}
}

void shuffleDeck(){
	for(int i = 0; i < deck.size();i++){
		Card temp = deck[i];
		int r = rand()% deck.size();
		deck[i] = deck[r];
		deck[r] = temp;
	}
}

void checkDeck(){
	if(!deck.empty())
		return;


	for(int i =0; i< discardPile.size()-1;i++){
		deck.push_back(discardPile[i]);
	}

	discardPile.erase(discardPile.begin(), discardPile.end()-2);

	shuffleDeck();
}

// TODO  expand beyond 2p

void playerTurn(){
	string val = discardPile.back().val;
	string color = curColor;
	string input;
	
	bool placeCard = false;
	
	if(val == "takeTwo"){
		for(int i=0;i<2;i++){
			checkDeck();
			playerHand.push_back(deck.back());
			deck.pop_back();
		}
		cout << "Oh no! You took two cards from the deck" << endl;
	}
	else if(val == "takeFour"){
		for(int i=0;i<4;i++){
			checkDeck();
			playerHand.push_back(deck.back());
			deck.pop_back();
		}
		cout << "Oh no! You took four cards from the deck" << endl;
	}

	cout << "Your hand is:"<<endl;
	for(int i=0;i<playerHand.size();i++){
		cout << i+1 << ". " << playerHand[i].color << " " << playerHand[i].val<<endl;
	}

	cout << "Current card on top of the pile is a " << color<< " " << val<<endl;
	cout << "Play a card or take a card from the deck"<<endl;
	cin >> input;
	while(stoi(input) < 0 || stoi(input) > playerHand.size() || (input != "0" && playerHand[stoi(input)-1].color != curColor && playerHand[stoi(input)-1].color != "wild" && playerHand[stoi(input)-1].val != discardPile.back().val)){
		cout << "Select a valid card or 0 to take from deck"<<endl;
		cin >> input;
	}
	if(input == "0"){
		int cardsTaken = 1;
		
		while(1){
			checkDeck();
			playerHand.push_back(deck.back());
			deck.pop_back();

			if(playerHand.back().color == color || playerHand.back().val == val || playerHand.back().color == "wild"){
				discardPile.push_back(playerHand.back());
				playerHand.pop_back();
				cout << "You took " << cardsTaken << " cards and finally got and discarded a " << discardPile.back().color << " " << discardPile.back().val<<endl;
				break;
			}
			cardsTaken++;
		}
	}
	else
	{
		discardPile.push_back(playerHand[stoi(input)-1]);
		playerHand.erase(playerHand.begin()+stoi(input)-1);
	}
	
	// apply effects of card played
	curColor = discardPile.back().color;
	if(discardPile.back().val == "reverse" || discardPile.back().val == "skip"){
		bTurn = !bTurn;
	}

	if(discardPile.back().color == "wild"){
		cout << "Your hand is:"<<endl;
		for(int i=0;i<playerHand.size();i++){
			cout << i+1 << ". " << playerHand[i].color << " " << playerHand[i].val<<endl;
		}
		cout << "What color do you want?"<<endl;
		cout << "('blue', 'green', 'red', 'yellow')"<<endl;
		cin >> curColor;
		while(curColor != "red" && curColor != "green" && curColor != "yellow" && curColor!= "blue"){
			cout << "Choose a valid color"<<endl;
			cin >> curColor;
		}
		cout << "The color is now " << curColor<<endl;
	}
}

void CPUTurn(){
	string val = discardPile.back().val;
	string color = curColor;
	bool placeCard = false;

	if(val == "takeTwo"){
		for(int i=0;i<2;i++){
			checkDeck();
			CPUHand.push_back(deck.back());
			deck.pop_back();
		}
	}
	else if(val == "takeFour"){
		for(int i=0;i<4;i++){
			checkDeck();
			CPUHand.push_back(deck.back());
			deck.pop_back();
		}
	}

	//check for card in hand
	for(int i=0;i < CPUHand.size();i++){
		//if card found, play it
		if(CPUHand[i].color == color || CPUHand[i].val == val || CPUHand[i].color =="wild"){
			discardPile.push_back(CPUHand[i]);
			CPUHand.erase(CPUHand.begin()+i);
			placeCard = true;
			break;
		}
	}
	
	if (!placeCard){
		cout << "The CPU had to take from deck!"<<endl;
	}

	// else take card from deck until a card can be played
	while(!placeCard){
		checkDeck();
		CPUHand.push_back(deck.back());
		deck.pop_back();

		if(CPUHand.back().color == color || CPUHand.back().val == val || CPUHand.back().color == "wild"){
			discardPile.push_back(CPUHand.back());
			CPUHand.pop_back();
			placeCard = true;
		}
	}

	// apply effects of card played
	curColor = discardPile.back().color;
	if(discardPile.back().val == "reverse" || discardPile.back().val == "skip"){
		bTurn = !bTurn;
	}

	if(discardPile.back().color == "wild"){
		int newColor = rand()%4;
		switch(newColor){
			case 0:
				curColor = "red";
				break;
			case 1:
				curColor = "yellow";
				break;
			case 2:
				curColor = "green";
				break;
			case 3:
				curColor = "blue";
		};

		cout << "The CPU played a " << discardPile.back().val << " wild card!"<<endl;
		cout << "The color is now " << curColor<<endl;
	}else{
		cout << "The CPU played a " << discardPile.back().color << " " << discardPile.back().val<<endl;
	}
}

int main(){
	srand((unsigned) time(0));

	bool bGameOver = false;
	bool winner;

	initializeDeck();
	shuffleDeck();

	cout << "Welcome to Uno!"<< endl<< endl;

	// deal cards to hands from deck
	for(int i = 0; i<7;i++){
		playerHand.push_back(deck.back());
		deck.pop_back();
		CPUHand.push_back(deck.back());
		deck.pop_back();
	}

	// reveal first discard
	discardPile.push_back(deck.back());
	deck.pop_back();
	curColor = discardPile[0].color;
	while(curColor == "wild"){
		deck.push_back(discardPile[0]);
		discardPile.pop_back();
		shuffleDeck();
		discardPile.push_back(deck.back());
		deck.pop_back();
		curColor = deck[0].color;
	}

	if(discardPile[0].val == "skip" || discardPile[0].val == "reverse"){
		bTurn = !bTurn;
	}
	
	cout <<"The first card drawn is a " << discardPile[0].color << " " << discardPile[0].val<<endl<< endl;


	// game loop
	while(!bGameOver){
		if(bTurn){
			playerTurn();
			if(playerHand.size() > 1){
				cout<< "You have " << playerHand.size() << " cards left!"<<endl<<endl<<endl;
			}else if(playerHand.size() == 1){
				cout << "UNO!"<<endl<<endl<<endl;
			}
			else{
				bGameOver = true;
				winner = true;
			}
		}
		else{
			CPUTurn();
			if(CPUHand.size() > 1){
				cout<< "CPU has " << CPUHand.size() << " cards left!"<<endl<<endl;
			}else if(CPUHand.size() == 1){
				cout << "UNO!"<<endl<<endl;
			}
			else{
				bGameOver = true;
				winner = false;
			}
		}
		bTurn = !bTurn;
	}

	// reveal winner
	if(winner){
		cout << "Congratulations! You won!"<<endl;
	}else{
		cout << "You lost! Better luck next time!"<<endl;
	}

	return 0;
}
