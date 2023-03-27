#include <iostream>
#include <string>

//examples for Dependency inversion principle

// Abstract class representing a notification method
class NotificationMethod {
public:
    virtual void sendNotification(std::string message) = 0;
};

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

int main() {
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

    // Clean up memory
    delete emailMethod;
    delete smsMethod;

    return 0;
}

