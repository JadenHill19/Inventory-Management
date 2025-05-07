//  Inventory & Order Management System
//
//  Created by Jaden Hill on 2/24/25.
//
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <queue>
#include <fstream>


//Product Catalog Service
class ProductCatalog{
    public:
    void addProduct(int id, const std::string& name, double price, int stock){
        products[id]={name, price, stock, 0}; //map for products the 0 is sold
    }
    
    void removeProduct(int id){
        products.erase(id);
    }
    
    void updateProduct(int id, double price, int stock){
        if(products.find(id)== products.end()){ //check if product exist
            std::cout<<"The product ID does not exist"<< std::endl;
        }
        else{
            products[id].price=price;  //update product details
            products[id].stock=stock;
        }
    }
    
    bool isAvailable(int id, int quantity){
        return products.find(id) != products.end() && products[id].stock>=quantity; // returns product and stock
    }
     
    void updateStock(int id, int quantity){
        if(isAvailable(id , quantity)){ // checks if item in stock
            products[id].stock-=quantity; //update stock
            products[id].sold+=quantity; //update amount sold
        }
    }
    void displayProducts(){
        for(const auto&[id, products]: products){  // Using a range-based for loop to interate through the map
            std::cout<<"ID:"<< id << std::endl;
            std::cout<<"Name of Product:"<< products.name<< std::endl;
            std::cout<<"price:"<< products.price<< std::endl;
            std::cout<<"Sold:"<< products.sold<< std::endl;
            std::cout<<"Stock:"<< products.stock<< std::endl;
        }
    }
    
    void saveInventoryToFile(const std::string& filename){
        std::ofstream file(filename);//open file
        for(const auto&[id, products]: products){// Using a range-based for loop to interate through the map
            file<< id <<","<<products.name<<","<< products.price<<","<<products.sold<<","<<products.stock<<std::endl; //inputs product details to file
        }
        file.close();
        std::cout << "Inventory saved to " << filename << std::endl;
    }
     
    void loadInventoryFromFile(const std::string& filename){
        std:: ifstream file(filename);//read from file
        if(file.is_open()){
            int id, stock, sold;
            double price;
            std::string name;
            while(file >> id >> name >> price >> sold >> stock){//reads through everyline of file
                products[id]={name , price, sold, stock}; //input values from files into keys
            }
            file.close();
        }
    }
    
    void displayBestSellingProducts(){
        //interate through all products, check how much sold better than avg then display those
        double totalSold=0;
        int count=0;
        double avgsold;
        
        for(const auto&[id, products]: products){
            totalSold+=products.sold;
            count++;
        }
        
        if(count>0){
            avgsold= totalSold/count;
        }
        
        else{
            avgsold=0;
        }
        std::cout<<"Best Selling Products"<<std::endl;
        for(const auto&[id, products]: products){
            if(products.sold>avgsold){
                std::cout<<"ID:"<< id<<","<<"Product:"<< products.name<< ","<< "Price:"<< products.price<<","<<"Amount Sold:"<<products.sold<<std::endl;
            }
        }
    }
    
    private:
            struct Product{
                std::string name;
                double price;
                int stock;
                int sold;
            };
                std::unordered_map<int ,Product> products; //intializing unordered map
};

// Order Processing Service
class OrderProcessing {
public:
    void placeOrder(ProductCatalog& catalog, int id, int quantity){
        if(catalog.isAvailable(id, quantity)){
            catalog.updateStock(id,quantity);
            shippingQueue.push(id);
            std::cout<<"Your order has been placed for:"<<id<<std::endl;
        }
        else
            std::cout<<"Sorry we are currenlty out of Stock for this item"<<std::endl;
    }
    void processShipping(){
        if(!shippingQueue.empty()) {
            std::cout<<"Shipping order in process for ID:"<<shippingQueue.front()<<std::endl;
            shippingQueue.pop();
        }
        else
            std::cout<<"No orders are currently availble"<<std::endl;
    }
private:
    std::queue<int> shippingQueue;
    
};

int main() {
    ProductCatalog catalog;
    OrderProcessing orderProcessing;
    catalog.loadInventoryFromFile("inventory.txt");
    
    int choice, id, stock, quantity;
    double price;
    std::string name;
    
    while(true){
        std::cout << "\n1. Add Product\n2. Remove Product\n3. Update Product\n4. View Products\n5. Save Inventory\n6. Place Order\n7. Ship Order\n8. View Best-Selling Products\n9. Quit\nEnter choice: ";
        std::cin >> choice;
        switch (choice) {
                    case 1:
                        std::cout << "Enter ID, Name, Price, Stock: ";
                        std::cin >> id >> name >> price >> stock;
                        catalog.addProduct(id, name, price, stock);
                        break;
                    case 2:
                        std::cout << "Enter Product ID to remove: ";
                        std::cin >> id;
                        catalog.removeProduct(id);
                        break;
                    case 3:
                        std::cout << "Enter Product ID, new Price, new Stock: ";
                        std::cin >> id >> price >> stock;
                        catalog.updateProduct(id, price, stock);
                        break;
                    case 4:
                        catalog.displayProducts();
                        break;
                    case 5:
                        catalog.saveInventoryToFile("inventory.txt");
                        break;
                    case 6:
                        std::cout << "Enter Product ID and Quantity to order: ";
                        std::cin >> id >> quantity;
                        orderProcessing.placeOrder(catalog, id, quantity);
                        break;
                    case 7:
                        orderProcessing.processShipping();
                        break;
                    case 8:
                        catalog.displayBestSellingProducts();
                        break;
                    case 9:
                        return 0;
                    default:
                        std::cout << "Invalid choice. Try again." << std::endl;
                }
            }
            return 0;
}
    
