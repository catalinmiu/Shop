#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <algorithm>

using namespace std;

//enum products{flour, cabbage};

class Product {
protected:
    string name;
    float cost;
    float price;
    float quantity;
    string unitType;
    friend class Stock;
    int points;

public:
    Product(string name = "", float cost = 0, float price = 0, float quantity = 0, string unitType = "") {
        this->name = name;
        this->cost = cost;
        this->price = price;
        this->unitType = unitType;
        this->quantity = quantity;
    }

    //void setName(string name) {this->name = name
    friend void setName(Product &P,string name)
    {
        P.name=name;
    }
    string getName() {return this->name;}

    void setCost(float cost) {this->cost = cost;}

    float getCost() {return this->cost;}

    void setPrice(float price) {this->price = price;}

    float getPrice() {return this->price;}

    void setUnitType(string unitType) {this->unitType = unitType;}

    string getUnitType() {return this->unitType;}

    void setQuantity(float quantity) {this->quantity = quantity;}

    float getQuantity() {return this->quantity;}

    void setPoints(int points) {this->points = points;}

    int getPoints() {return this->points;}

    bool operator<(const Product& other) const
    {
        if(points != other.points) {
            return points > other.points;
        }
        return price-cost>other.price-other.cost;
    }
};

class Cabbage:public Product {
public:
    Cabbage(string name="cabbage", float cost = 0, float price = 0, float quantity = 0, string unitType = "piece") {
        this->name = name;
        this->cost = cost;
        this->price = price;
        this->unitType = unitType;
        this->quantity = quantity;
    }
};

class Flour:public Product {
    string floorQuality;

public:
    Flour(string name = "flour", float cost = 0, float price = 0, float quantity = 0, string floorQuality = "", string unitType = "weight") {
        this->name = name;
        this->cost = cost;
        this->price = price;
        this->unitType = unitType;
        this->quantity = quantity;
        this->floorQuality = floorQuality;
        this->points = 0;
    }

    string getFloorQuality() {return this->floorQuality;}

};

class Stock {
    vector<Flour> listOfFlours;
    vector<Cabbage> listOfCabbages;

    string returnNextParam(int* poz1, int* poz2, const string line) {
        *poz1 = *poz2 + 1;
        *poz2 = line.find('-', *poz1);
        return line.substr(*poz1,*poz2-*poz1);
    }

    string toLower(string name) {
        for(int i=0;i<name.size();i++) {
            if(name.at(i)>='A' && name.at(i)<='Z') {
                name.at(i)+=32;
            }
        }
        return name;
    }
    void sor() {
        sort(listOfFlours.begin( ), listOfFlours.end( ), [ ]( const Flour& lhs, const Flour& rhs )
        {
           return lhs < rhs;
        });
    }

public:
    Stock() {
        init();
    }

    void init() {
        ifstream in("stock2.txt");
        string line;
        int poz1, poz2;
        string object, name, cost, price, quantity, flourQuality, unitType;
        while(getline(in, line)) {
            poz2 = -1; // When returnNextParam function will be called, poz1 will become poz2+1=0
            name = returnNextParam(&poz1, &poz2, line);
            cost = returnNextParam(&poz1, &poz2, line);
            price = returnNextParam(&poz1, &poz2, line);
            quantity = returnNextParam(&poz1, &poz2, line);
            string auxName;
            auxName = toLower(name);
            if(auxName.find("flour")!=-1) {
                flourQuality = returnNextParam(&poz1, &poz2, line);
                unitType = returnNextParam(&poz1, &poz2, line);
                Flour flour(name, stof(cost), stof(price), stof(quantity), flourQuality, unitType);
                listOfFlours.push_back(flour);
            }
            if(auxName.find("cabbage")!=-1) {
                unitType = returnNextParam(&poz1, &poz2, line);
                Cabbage cabbage(name, stof(cost), stof(price), stof(quantity), unitType);
                listOfCabbages.push_back(cabbage);
            }
        }
        in.close();
    }

    float getFlourStock() {
        float totalQuantity = 0;
        for(int i=0;i<listOfFlours.size();i++) {
            totalQuantity += listOfFlours.at(i).quantity;
        }
        return totalQuantity;
    }

    float getCabbageStock() {
        float totalQuantity = 0;
        for(int i=0;i<listOfCabbages.size();i++) {
            totalQuantity += listOfCabbages.at(i).quantity;
        }
        return totalQuantity;
    }

    vector<Flour>& getListOfFlours() {return this->listOfFlours;}
    vector<Cabbage>& getListOfCabbages() {return this->listOfCabbages;}

    friend class ShopMan;
};

class ShoppingBasket {
    vector<Flour> listOfFlours;
    vector<Cabbage> listOfCabbages;
public:
    void addFlour(Flour flour) {
        listOfFlours.push_back(flour);
    }

    void addCabbage(Cabbage cabbage) {
        listOfCabbages.push_back(cabbage);
    }
    vector<Flour> getListOfFlours() {return this->listOfFlours;}
    vector<Cabbage> getListOfCabbages() {return this->listOfCabbages;}
};

class ShoppingList {
    vector<string> shoppingListNames;

    string toLower(string name) {
        for(int i=0;i<name.size();i++) {
            if(name.at(i)>='A' && name.at(i)<='Z') {
                name.at(i)+=32;
            }
        }
        return name;
    }

public:
    void init() {
        ifstream in("shoppingList.txt");
        string line;
        int poz1, poz2;
        string productName;
        while(getline(in, line)) {
            productName = toLower(line);
            shoppingListNames.push_back(productName);
        }
        in.close();
    }
    vector<string> getShoppingListNames() {
        return this->shoppingListNames;
    }
};

class Person {
protected:
    string name;
public:
    Person(string name = "") {
        this->name = name;
    }
};

class Client:Person {
    float amountOfMoney;
    ShoppingList shoppingList;
    ShoppingBasket shoppingBasket;
    float amountOfMoneyInShoppingBasket;
public:
    Client(string name = "Client", float amountOfMoney = 0) {
        this->name = name;
        this->amountOfMoney = amountOfMoney;
        this->amountOfMoneyInShoppingBasket = 0;
        shoppingList.init();
    }

    ShoppingList getShoppingList() {
        return this->shoppingList;
    }
    void setShoppingList(ShoppingList shoppingList) {
        this->shoppingList = shoppingList;
    }
    ShoppingBasket& getShoppingBasket() {
        return this->shoppingBasket;
    }
    float getAmountOfMoney() {
        return this->amountOfMoney;
    }
    float getAmountOfMoneyInShoppingBasket() {
        return this->amountOfMoneyInShoppingBasket;
    }
    friend void setAmountOfMoney(Client &C, float amountOfMoney) {
        C.amountOfMoney = amountOfMoney;
    }
    friend void setAmountOfMoneyInShoppingBasket(Client &C, float amountOfMoneyInShoppingBasket) {
        C.amountOfMoneyInShoppingBasket = amountOfMoneyInShoppingBasket;
    }
};

class ShopMan:Person {
    //ShoppingList clientList, ClientShoppingBasket;
    bool isAProductThatWeHave(string product) {
        if(product.find("flour")!=-1) return true;
        if(product.find("cabbage")!=-1) return true;
        return false;
    }
    string returnNextParam(int* poz1, int* poz2, const string line, char ch) {
        *poz1 = *poz2 + 1;
        *poz2 = line.find(ch, *poz1);
        return line.substr(*poz1,*poz2-*poz1);
    }

    int getNoOfWords(string line) {
        int no=1;
        for(int i=0;i<line.size();i++) {
            if(line.at(i) == ' ') {
                no++;
            }
        }
        return no;
    }
    string toLower(string name) {
        for(int i=0;i<name.size();i++) {
            if(name.at(i)>='A' && name.at(i)<='Z') {
                name.at(i)+=32;
            }
        }
        return name;
    }

public:
    ShopMan(string name = "") {
        this->name = name;
    }
    void putProductsBackInStock(Stock& stock, ShoppingBasket clientShoppingBasket) {
        for(int i=0;i<clientShoppingBasket.getListOfFlours().size();i++) {
            for(int j=0;j<stock.getListOfFlours().size();j++)
                if(clientShoppingBasket.getListOfFlours().at(i).getName() == stock.getListOfFlours().at(j).getName())
                    stock.getListOfFlours().at(j).setQuantity(stock.getListOfFlours().at(j).getQuantity()+1);
        }
        for(int i=0;i<clientShoppingBasket.getListOfCabbages().size();i++) {
            for(int j=0;j<stock.getListOfCabbages().size();j++)
                if(clientShoppingBasket.getListOfCabbages().at(i).getName() == stock.getListOfCabbages().at(j).getName())
                    stock.getListOfCabbages().at(j).setQuantity(stock.getListOfCabbages().at(j).getQuantity()+1);
        }
    }
    void purposeMostProfitableChoices(Stock& stock, Client& client) {
        int poz1, poz2;
        string line, param, quantity, productName;
        int index=1;
        for(int i=0;i<client.getShoppingList().getShoppingListNames().size();i++) {
            for(int j=0;j<stock.getListOfFlours().size();j++) {
                stock.getListOfFlours().at(j).setPoints(0);
            }
            for(int j=0;j<stock.getListOfCabbages().size();j++) {
                stock.getListOfCabbages().at(j).setPoints(0);
            }
            if(!isAProductThatWeHave(client.getShoppingList().getShoppingListNames().at(i))) {
                cout<<"We don't have "<<client.getShoppingList().getShoppingListNames().at(i)<<endl;
                continue;
            }
            else {
                line = client.getShoppingList().getShoppingListNames().at(i);
                if(line.find("flour")!=-1) {
                    for(int j=0;j<stock.getListOfFlours().size();j++) {
                        int noOfWords = getNoOfWords(line); //return no. of words
                        poz2 = -1;
                        productName = returnNextParam(&poz1, &poz2, line, '-');
                        quantity = returnNextParam(&poz1, &poz2, line, '-');
                        poz2 = -1;
                        while(noOfWords>0) {
                            string param = returnNextParam(&poz1, &poz2, productName, ' ');
                            if(toLower(stock.getListOfFlours().at(j).getName()).find(param)!=-1) {
                                stock.getListOfFlours().at(j).setPoints(stock.getListOfFlours().at(j).getPoints() + 1);
                            }
                            //cout<<stock.getListOfFlours().at(j).getFloorQuality();
                            if(toLower(stock.getListOfFlours().at(j).getFloorQuality()).find(param)!=-1) {
                                stock.getListOfFlours().at(j).setPoints(stock.getListOfFlours().at(j).getPoints() + 1);
                            }
                            noOfWords--;
                        }
                    }
                    stock.sor();
                    float quantityAux = stof(quantity);
                    for(int j=0;j<stock.getListOfFlours().size();j++) {
                        float quantityOfFavProduct = stock.getListOfFlours().at(j).getQuantity();

                        while(stock.getListOfFlours().at(j).getQuantity()>0 && quantityAux>0 && client.getAmountOfMoney()-client.getAmountOfMoneyInShoppingBasket()>stock.getListOfFlours().at(j).getPrice()) {
                            client.getShoppingBasket().addFlour(stock.getListOfFlours().at(j));
                            /*cout<<client.getAmountOfMoney()-client.getAmountOfMoneyInShoppingBasket()<<" "<<stock.getListOfFlours().at(j).getPrice();
                            cout<<endl;*/
                            setAmountOfMoneyInShoppingBasket(client, client.getAmountOfMoneyInShoppingBasket()+stock.getListOfFlours().at(j).getPrice());
/*
                            cout<<index<<". "<<stock.getListOfFlours().at(j).getName()<<endl;
                            cout<<client.getAmountOfMoney()-client.getAmountOfMoneyInShoppingBasket()<<" "<<stock.getListOfFlours().at(j).getPrice();
                            cout<<endl;*/
                            index++;
                            stock.getListOfFlours().at(j).setQuantity(stock.getListOfFlours().at(j).getQuantity()-1);
                            quantityAux--;
                        }
                    }
                }

                else if(line.find("cabbage")!=-1) {
                    for(int j=0;j<stock.getListOfCabbages().size();j++) {
                        int noOfWords = getNoOfWords(line); //return no. of words
                        poz2 = -1;
                        productName = returnNextParam(&poz1, &poz2, line, '-');
                        quantity = returnNextParam(&poz1, &poz2, line, '-');
                        poz2 = -1;
                        while(noOfWords>0) {
                            string param = returnNextParam(&poz1, &poz2, productName, ' ');
                            if(toLower(stock.getListOfCabbages().at(j).getName()).find(param)!=-1) {
                                stock.getListOfCabbages().at(j).setPoints(stock.getListOfCabbages().at(j).getPoints() + 1);
                            }

                            noOfWords--;
                        }
                    }
                    stock.sor();
                    float quantityAux = stof(quantity);
                    for(int j=0;j<stock.getListOfCabbages().size();j++) {
                        float quantityOfFavProduct = stock.getListOfCabbages().at(j).getQuantity();

                        while(stock.getListOfCabbages().at(j).getQuantity()>0 && quantityAux>0 && client.getAmountOfMoney()-client.getAmountOfMoneyInShoppingBasket()>stock.getListOfCabbages().at(j).getPrice()) {
                            client.getShoppingBasket().addCabbage(stock.getListOfCabbages().at(j));
                            setAmountOfMoneyInShoppingBasket(client, client.getAmountOfMoneyInShoppingBasket()+stock.getListOfCabbages().at(j).getPrice());
                            //cout<<index<<". "<<stock.getListOfCabbages().at(j).getName()<<endl;
                            index++;
                            stock.getListOfCabbages().at(j).setQuantity(stock.getListOfCabbages().at(j).getQuantity()-1);
                            quantityAux--;
                        }
                    }
                }
            }
        }
    }

};

int main()
{
    float amount;
    cout<<"How much money do you have?";
    cin>>amount;
    Client client("client", amount);
    Stock stock;
    ShopMan shopMan("ShopMan");
    cout<<"ShoppingList:"<<endl;
    for(int i=0;i<client.getShoppingList().getShoppingListNames().size();i++) {
        cout<<i+1<<". "<<client.getShoppingList().getShoppingListNames().at(i)<<endl; //afisare shoppingList
    }
    cout<<endl<<"Stock:"<<endl;
    int index = 0;
    for(int i=0;i<stock.getListOfFlours().size();i++) {
        cout<<++index<<". "<<stock.getListOfFlours().at(i).getName()<<endl;
    }
    for(int i=0;i<stock.getListOfCabbages().size();i++) {
        cout<<++index<<". "<<stock.getListOfCabbages().at(i).getName()<<endl;
    }

    shopMan.purposeMostProfitableChoices(stock, client);
    cout<<endl<<"Ai in cos: "<<endl;
    for(int i=0;i<client.getShoppingBasket().getListOfFlours().size();i++) {
        cout<<client.getShoppingBasket().getListOfFlours().at(i).getName()<<endl;
    }
    for(int i=0;i<client.getShoppingBasket().getListOfCabbages().size();i++) {
        cout<<client.getShoppingBasket().getListOfCabbages().at(i).getName()<<endl;
    }
    cout<<"Total:"<<client.getAmountOfMoneyInShoppingBasket()<<endl;
    cout<<"Do you want to buy all this list?Y/N";
    string answer;
    cin>>answer;
    if(answer=="Y" or answer=="y") {
        setAmountOfMoney(client, client.getAmountOfMoney()-client.getAmountOfMoneyInShoppingBasket());

    }
    else {
        shopMan.putProductsBackInStock(stock, client.getShoppingBasket());
    }
    cout<<"Goodbye!"<<endl;

    cout<<"Client - AmountOfClient: "<<client.getAmountOfMoney()<<endl<<endl;
    cout<<"Shop stock: "<<endl;
    cout<<"- Flours:"<<stock.getFlourStock()<<endl;
    cout<<"- Cabbage:"<<stock.getCabbageStock()<<endl;

    return 0;
}

