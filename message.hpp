#include <iostream>
#include <string>
#include <cstring>
#ifndef MESSAGE_HPP // include guard
#define MESSAGE_HPP

class Message{
    
    public:
        Message(): bodyLength_(0) {} // member initializer
        enum {maxBytes = 512};
        enum {header = 4};

        Message(std:: string message){
            bodyLength_ = getNewBodyLength(message.size());
            encodeHeader();
            // copies the actual message into data
            std::memcpy(data+header, message.c_str(), bodyLength_);
        }

        // validates the length of the message
        size_t getNewBodyLength(size_t newLength){
            if(newLength > maxBytes){
                return maxBytes;
            }
            return newLength;
        }

        // returns entire data ie. header + body
        std::string getData(){
            int length= header + bodyLength_;
            std::string result(data, length);
            return result;
        }

        // returns only bidy
        std::string getBody(){
            std::string dataStr= getData();
            std::string result = dataStr.substr(header, bodyLength_);
            return result;
        }

        bool decodeHeader(){
            char new_header[header+1]= "";
            strncpy(new_header, data, header);
            new_header[header]= '\0';
            int headervalue= atoi(new_header);

            // if somehow the max limit of data is by-passed
            if(headervalue>maxBytes){
                bodyLength_= 0;
                return false;
            }
            bodyLength_= headervalue;
            return true;
        }
        
        // sets the header in the data
        void encodeHeader(){
            char new_header[header+1] = "";
            sprintf(new_header, "%4d", static_cast<int>(bodyLength_));
            std::memcpy(data, new_header, header);
        }

    private:
        // data buffer
        char data[header+maxBytes];
        size_t bodyLength_;
    
};


#endif MESSAGE_HPP