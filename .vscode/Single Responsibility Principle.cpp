#include <iostream>
#include <string>

// OrderDetails class, responsible for storing the order details and calculating the total price
class OrderDetails {
public:
    OrderDetails(int quantity, double price)
        : quantity_(quantity), price_(price) {}

    int getQuantity() const {
        return quantity_;
    }

    double getPrice() const {
        return price_;
    }

    double getTotalPrice() const {
        return quantity_ * price_;
    }

private:
    int quantity_;
    double price_;
};

// EmailNotification class, responsible for sending an email notification to the customer
class EmailNotification {
public:
    void sendEmailNotification(std::string email) {
        std::cout << "Email notification sent to " << email << std::endl;
    }
};

// Order class, responsible for creating and processing orders
class Order {
public:
    Order(std::string product, int quantity, double price, std::string email)
        : orderDetails_(quantity, price), email_(email), product_(product) {}

    std::string getProduct() const {
        return product_;
    }

    int getQuantity() const {
        return orderDetails_.getQuantity();
    }

    double getPrice() const {
        return orderDetails_.getPrice();
    }

    double getTotalPrice() const {
        return orderDetails_.getTotalPrice();
    }

    void processOrder() {
        // Process the order
        std::cout << "Processing order..." << std::endl;

        // Send email notification to the customer
        EmailNotification emailNotification;
        emailNotification.sendEmailNotification(email_);

        std::cout << "Order processed successfully" << std::endl;
    }

    void processSMSOrder() {}

private:
    OrderDetails orderDetails_;
    std::string email_;
    std::string product_;
};

int main() {
    Order order("Product A", 2, 10.99, "johndoe@example.com");

    std::cout << "Product: " << order.getProduct() << std::endl;
    std::cout << "Quantity: " << order.getQuantity() << std::endl;
    std::cout << "Price: " << order.getPrice() << std::endl;
    std::cout << "Total Price: " << order.getTotalPrice() << std::endl;

    order.processOrder();

    return 0;
}


