<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Default.aspx.cs" Inherits="_Default" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title>Texas Delivery Services</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <link href="StyleSheet.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="ContactInfo.js" ></script>
</head>
<body  onload="loadDoc()">
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
                       <h1>The Service You can Trust</h1>
                       <p>Enjoy a 1-to-1 relationship without your very own TEXAS delivery Service, who delivers for you each time 
                           and learns your preferences. </p>
                       <p>Get your favourite products from store of your preference and get delivered right to your door.</p>
                       <p>We offer a range of other services including Medical deliveries Legal courier deliveries etc. </p>
                       <p>It's all backed by our 100% Stress-Free Guarantee!!!</p>
                </div>
                <div  class ="post_content1">
                    <img src="Images/2.jpg" alt ="" />
                    <img src="Images/12.jpg" alt ="" />
                    <img src="Images/13.jpg" alt ="" />   
                </div>
            </div>
        </div>
        <table>
            <tr>
                <td>
                    <asp:AdRotator id="ads1" runat="server" AdvertisementFile="adsCustomerService.xml" Target="_blank" KeywordFilter="delivery1" Width ="340px" Height="300px" />
                </td>
                <td>
                    <asp:AdRotator id="ads2" runat="server" AdvertisementFile="adsCustomerService.xml" Target="_blank" KeywordFilter="delivery2" Width ="340px" Height="300px" />
                </td>
                <td>
                    <asp:AdRotator id="ads3" runat="server" AdvertisementFile="adsCustomerService.xml" Target="_blank" KeywordFilter="delivery1" Width ="340px" Height="300px" />
                </td>
                <td>
                    <asp:AdRotator id="ads4" runat="server" AdvertisementFile="adsCustomerService.xml" Target="_blank" KeywordFilter="delivery2" Width ="340px" Height="300px" />
                </td>
            </tr>
        
        </table>
    </div>   
    </form>
</body>
</html>
