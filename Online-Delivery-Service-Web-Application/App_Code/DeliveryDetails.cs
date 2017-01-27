using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

/// <summary>
/// This class will store delivery details of each user.
/// </summary>

public class DeliveryDetails
{
    DateTime requestDate;
    String pickupAddress; //with the fields in the
    String receipientAddress; //’New Delivery Details’ page.
    String receipientPhone;
    String description;

    public DeliveryDetails(DateTime requestDate, String pickupAddress, String receipientAddress, String receipientPhone,String description)
    {
        this.requestDate = requestDate;
        this.pickupAddress = pickupAddress;
        this.receipientAddress = receipientAddress;
        this.receipientPhone = receipientPhone;
        this.description = description;
    }

    public DateTime RequestDate
    {
        get
        {
            return requestDate;
        }
        
    }
    public String PickupAddress
    {
        get 
        { 
            return pickupAddress; 
        }
        set
        { 
            pickupAddress = value; 
        }
    }

    public String ReceipientAddress
    {
        get
        {
            return receipientAddress; 
        }
        set 
        { 
            receipientAddress = value;
        }
    }

    public String ReceipientPhone
    {
        get 
        { 
            return receipientPhone;
        }
        set 
        { 
            receipientPhone = value;
        }
    }


    public String Description
    {
        get 
        { 
            return description; 
        }
        set
        {
            description = value; 
        }
    }

}