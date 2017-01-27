<%@ Page Language="C#" AutoEventWireup="true" CodeFile="ServiceArea.aspx.cs" Inherits="_Default" %>

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
                   <img src="Images/20.jpg" style="width:100%; height:16%" />
                       <p>Texas Delivery Service is your Demand Specialist and your Instant Solution to your same day shipping needs.
                           We are the choice of hundreds of customers around the city and around the state - let us be your choice. LET US 
                           show you why, "You Can Count Us - We're Texans Serving Texans 24 Hours a Day!!".  
                   </p>                       
                    <table>
                            <tr>
                                <td colspan="3">
                                   <h1>We are Serving following Areas.</h1>
                                </td>
                           </tr>
                            <tr><td>
                                <table id="contactDetails" class="serviceArea"></table></td>
                                
                            </tr>
                   </table>
                </div>
            </div>
        </div>
    </div>   
    </form>
</body>
</html>
