<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Service.aspx.cs" Inherits="_Default" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title>Texas Delivery Services</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <link href="StyleSheet.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="ContactInfo.js" ></script>
</head>
<body onload="loadDoc()">
    <form id="form1" runat="server">
    <div class ="main">
        <div class ="banner">
            <img src ="Images/9.jpg" class="banner_img1" />
            <img src ="Images/banner1.png" class="banner_img2" />
        </div>
        
        <div class="menu_nav">
            <table>
                <tr>
                    <td><a href="Default.aspx"><span>Home</span></a></td>
                    <td><a href="Aboutus.aspx"><span>About Us</span></a></td>
                    <td><a href="Service.aspx"><span>Services</span></a></td>
		            <td><a href="Account.aspx"><span>Accounts</span></a></td>
		            <td><a href="Contactus.aspx"><span>Contact Us</span></a></td>
                    <td><a href="Resources.aspx"><span>Resources</span></a></td>
                    <td><a href="ServiceArea.aspx"><span>Service Areas</span></a></td>
                </tr>
            </table>
       </div>
       
       <div class ="mainbar">
           <div class="col_one">
               <div class ="article">
                   <table id="phoneDetails"></table>
               </div>
               <div class="article1">
                    <h1>WE ACCEPT</h1>
                    <img src="Images/6.jpg" alt ="" />
                </div>
           </div>
           <div class="col_two">
               <div class="post_content">
                   <img src="Images/21.jpg" style="width:100%; height:16%" /><!--<h1><u>SERVICES</u></h1>-->
                   <p>Texas Delivery Services is geared to meet a variety of challenges that your same day shipping needs might present. 
                       We have the knowledge and experience to set up programs to specifically meet or exceed both you and your client's
                       expectations of service, quality and on-time dependability. </p>                       
                    <table>
                            <tr>
                                <td>
                                    <img src="Images/17.jpg" height="250" width="250"/>
                                </td>
                                <td>
                                   <p style="color:#40752b; font-size:25px; font-weight:bold; font-family:Algerian">Here are a few of the specialized programs we offer:</p>
                                   <ul>
                                       <li>Grocery Delivery</li>
                                       <li>Medical Deliveries</li>
                                       <li>Legal Courier Deliveries</li>
                                   </ul>
                                </td>
                                <td>
                                    <img src="Images/16.jpg" height="250" width="250"/>
                                </td>
                               </tr>
                            </table>
                   <p>Each type of service is unique in its specific requirements of service and commitments and each is understood by our staff
                       of trained professionals. our commitment of 24 hours a day reliability to our clients is unsurpassed. Our performance is one
                       you can count on immediately!
                   </p>
                </div>
            </div>
        </div>
    </div>   
    </form>
</body>
</html>
