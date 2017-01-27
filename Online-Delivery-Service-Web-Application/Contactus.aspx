<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Contactus.aspx.cs" Inherits="_Default" %>

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
                           <h1>Please Email US All Your Concerns</h1>
                            <table>
                                <tr>
                                    <td>
                                        <asp:Label id="emailLabel" Text = "Enter Your Email Address:" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Brown"></asp:Label>
                                    </td>
                                    <td>
                                        <asp:TextBox id="emailText" runat="server" Width="400px" Height="25px" Font-Size ="20px"></asp:TextBox>
                                        <asp:RequiredFieldValidator id="validEmailReq" runat="server" 
                                                                ErrorMessage="Email Id is Required." 
                                                                ControlToValidate="emailText"  
                                                                ForeColor="Red"
                                                                ValidationGroup="emailMsgGroup"
                                                                Display ="Dynamic"/>
                                        <asp:RegularExpressionValidator ID="validEmailReg" runat="server"     
                                                    ErrorMessage="Kindly Enter Valid Email-Id. Eg. example@exmaple.com" 
                                                    ControlToValidate="emailText"     
                                                    ValidationExpression="^([\w\.\-]+)@([\w\-]+)((\.(\w){2,3})+)$"
                                                    ForeColor="Red"
                                                    ValidationGroup="emailMsgGroup"
                                                    Display ="Dynamic" />
                                    </td>
                       </tr>
                       <tr>
                           <td colspan ="2">
                            <asp:TextBox id="messageText" TextMode ="MultiLine" runat="server"  Width="90%" Height="220px" Font-Size ="20px" Text="Type your message here." />
                               <asp:RequiredFieldValidator id="validMsgreq" runat="server" 
                                                                ErrorMessage="Message is Required." 
                                                                ControlToValidate="messageText"  
                                                                ForeColor="Red"
                                                                ValidationGroup="emailMsgGroup"
                                                                Display ="Dynamic"/>
                           </td>
                      </tr>
                      <tr>
                           <td colspan ="2">
                                <asp:Button id="submitReq" Text="Submit Message" runat="server" OnClick="submitMesage" Width="250px" Height="35px" Font-Size ="22px" ForeColor="Brown" Font-Bold="true"></asp:Button>
                            </td>
                     </tr> 
                      </table>
                </div>
                <div  class ="post_content1">
                    <img src="Images/2.jpg" alt ="" />
                    <img src="Images/12.jpg" alt ="" />
                    <img src="Images/13.jpg" alt ="" />   
                </div>
            </div>
        </div>
    </div>   
    </form>
</body>
</html>
