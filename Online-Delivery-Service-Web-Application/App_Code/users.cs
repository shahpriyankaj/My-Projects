using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

/// <summary>
/// This class will store user related data.
/// </summary>
public class Users
{
    String fullName;
    String company;
    String mailingAddress;
    String phoneNumber;
    String emailAddress;
    int accessCode;
    List<DeliveryDetails> deliveryDetailsList;

    public Users(String fullName, String company, String mailingAddress, String phoneNumber, String emailAddress, int accessCode)
    {
        this.fullName = fullName;
        this.company = company;
        this.mailingAddress = mailingAddress;
        this.phoneNumber = phoneNumber;
        this.emailAddress = emailAddress;
        this.accessCode = accessCode;
        deliveryDetailsList = new List<DeliveryDetails>();
    }

    public String FullName //This is a property to the name field.
    {
        get 
        { 
            return fullName; 
        }
        set 
        { 
            fullName = value; 
        }
    }

    public String Company //This is a property to the company field.
    {
        get 
        { 
            return company; 
        }
        set 
        { 
            company = value; 
        }
    }

    public String MailingAddress //This is a mailingAddress to the company field.
    {
        get 
        { 
            return mailingAddress; 
        }
        set 
        { 
            mailingAddress = value;
        }
    }
    public String PhoneNumber //This is a phoneNumber to the company field.
    {
        get 
        {
            return phoneNumber; 
        }
        set 
        { 
            phoneNumber = value;
        }
    }
    public String EmailAddress //This is a property to the emailAddress field.
    {
        get 
        { 
            return emailAddress; 
        }
        set 
        { 
            emailAddress = value; 
        }
    }
    public int AccessCode //This is a property to the accessCode field.
    {
        get 
        {
            return accessCode;
        }
        set 
        { 
            accessCode = value; 
        }
    }



    public List<DeliveryDetails> DeliveryDetailsList //This is a property to the deliveryDetailsList field.
    {
        get 
        { 
            return deliveryDetailsList; 
        }
    }

}