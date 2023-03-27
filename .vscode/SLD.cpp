#include <iostream>
#include <string>

// Abstract class representing a notification method
class NotificationMethod {
public:
    virtual void sendNotification(std::string message) = 0;
};

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
// Class representing a notification sent via email
class EmailNotification : public NotificationMethod {
public:
    void sendNotification(std::string message) override {
        std::cout << "Sending email notification: " << message << std::endl;
        // Code for sending an email notification
    }
};

// Class representing a notification sent via SMS
class SMSNotification : public NotificationMethod {
public:
    void sendNotification(std::string message) override {
        std::cout << "Sending SMS notification: " << message << std::endl;
        // Code for sending an SMS notification
    }
};

// Class representing a notification service that sends notifications
class NotificationService {
public:
    NotificationService(NotificationMethod* method) : method_(method) {}

    void setNotificationMethod(NotificationMethod* method) {
        method_ = method;
    }

    void sendNotification(std::string message) {
        method_->sendNotification(message);
    }

private:
    NotificationMethod* method_;
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

        // Send notification to the customer

        // Create an email notification method
        NotificationMethod* emailMethod = new EmailNotification();

        // Create a notification service that uses email
        NotificationService emailService(emailMethod);

        // Send a notification via email
        emailService.sendNotification("Hello, world!");

        // Create an SMS notification method
        NotificationMethod* smsMethod = new SMSNotification();

        // Change the notification method of the service to SMS
        emailService.setNotificationMethod(smsMethod);

        // Send a notification via SMS
        emailService.sendNotification("Hello, world!");

    }

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


