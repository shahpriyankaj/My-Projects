<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Account.aspx.cs" Inherits="_Default" %>

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
        <asp:ValidationSummary runat="server" headertext="There were errors on the page:" />
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

                    <asp:Panel id = "radio123" runat="server" Visible="False">
                        <asp:RadioButtonList ID="accountRadio" runat="server" AutoPostBack="True"
                            onselectedindexchanged="accountIndexChanged"
                            RepeatDirection="Horizontal">
                            <asp:ListItem Text="I have an existing account" Value="existRadio" Selected="True"></asp:ListItem>
                            <asp:ListItem Text="I do not have an existing account" Value="newRadio" Selected="False"></asp:ListItem>
                        </asp:RadioButtonList>
                    </asp:Panel>
                   <br />
                    <!-- Existing Account-->
                   <asp:Panel id = "existAcount" runat="server">
                       <fieldset>
                           <legend class ="accountLegend">Existing Account</legend>
                           <br />
                           <table style="height:100%;width:100%">
                               <tr>
                                   <td><asp:Label id="emailLabel" Text = "Email Address:" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Brown"></asp:Label>
                                       <asp:Label id="atrik8" Text = "*" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Red"></asp:Label>
                                   </td>
                                   <td><asp:TextBox id="emailText" runat="server" Width="250px" Height="25px" Font-Size ="20px"></asp:TextBox>
                                        <asp:RequiredFieldValidator id="validEmailReq" runat="server" 
                                                            ErrorMessage="Email Id is Required." 
                                                            ControlToValidate="emailText"  
                                                            ForeColor="Red"
                                                            ValidationGroup="existAccountGroup"
                                                             Display ="Dynamic"/>
                                        <asp:RegularExpressionValidator ID="validEmailReg" runat="server"     
                                                ErrorMessage="Kindly Enter Valid Email-Id. Eg. example@exmaple.com" 
                                                ControlToValidate="emailText"     
                                                ValidationExpression="^([\w\.\-]+)@([\w\-]+)((\.(\w){2,3})+)$"
                                                ForeColor="Red"
                                                ValidationGroup="existAccountGroup"
                                                Display ="Dynamic" />
                                  </td>
                               </tr>
                               <tr>

                                    <td><asp:Label id="pwdLabel" Text = "Access Code:"  runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Brown"  ></asp:Label>
                                        <asp:Label id="atrik9" Text = "*" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Red"></asp:Label></td>
                                   
                                    <td><asp:TextBox id="pwdText" TextMode ="Password" runat="server" Width="250px" Height="25px" Font-Size ="20px"></asp:TextBox>
                                
                                    <asp:RequiredFieldValidator id="validPwdReq" runat="server" 
                                                            ErrorMessage="Access Code is Required." 
                                                            ControlToValidate="pwdText" 
                                                            ForeColor="Red"
                                                            ValidationGroup="existAccountGroup"
                                                            Display ="Dynamic"/>
                                        <asp:RegularExpressionValidator ID="validPwdReg" runat="server"    
                                                ErrorMessage="Access Code must be Six digits Long." 
                                                ControlToValidate="pwdText"     
                                                ValidationExpression="^[0-9]{6}$"
                                                ForeColor="Red" 
                                                ValidationGroup="existAccountGroup"
                                                Display ="Dynamic"/>

                                    </td>
                              </tr>
                               <tr>
                                   <td>
                                       <asp:Label id="errorL1"  runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Brown"  ></asp:Label>
                                   </td>
                               </tr>
                              <tr>
                                    <td><asp:linkButton Text="Forgot My Access Code" runat="server" Font-Size="20px" OnClick="get_access_code_Click"></asp:linkButton></td>
                                    <td><asp:Button id="logInButton" Text="Log In" runat="server" OnClick="logInAccount" Width="100px" Height="35px" Font-Size ="25px" ForeColor="Brown" Font-Bold="true"></asp:Button></td>
                              </tr>
                               <tr><td>&nbsp</td><td>&nbsp</td></tr>
                          </table>
                       </fieldset>
                       <br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br />
                   </asp:Panel>

                   <!-- Recover Access Code -->
                   <asp:Panel id = "recoverPassword" runat="server">
                       <fieldset>
                           <legend class = "recoverLegend">Recover Password</legend>
                           <br />
                           <table style="height:100%;width:100%">
                               <tr>
                                   <td><asp:Label id="emailLabel1" Text = "Email Address:" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Brown"></asp:Label>
                                       
                                   </td>
                                   <td><asp:TextBox id="emailText1" runat="server" Width="250px" Height="25px" Font-Size ="20px" AutoPostBack ="true" OnTextChanged="recover_password"></asp:TextBox>
                                       <asp:RequiredFieldValidator id="validEmailReq1" runat="server" 
                                                            ErrorMessage="Email Id is Mandatory." 
                                                            ControlToValidate="emailText1" 
                                                            ForeColor="Red"
                                                            ValidationGroup="recPwdGroup"
                                                            Display ="Dynamic"/>
                                       <asp:RegularExpressionValidator ID="validEmailReg1" runat="server"   
                                                ErrorMessage="Kindly Enter Valid Email-Id. Eg. example@exmaple.com" 
                                                ControlToValidate="emailText1"     
                                                ValidationExpression="^([\w\.\-]+)@([\w\-]+)((\.(\w){2,3})+)$"
                                                ForeColor="Red" 
                                                ValidationGroup="recPwdGroup"
                                                Display ="Dynamic"/>
                                  </td>
                               </tr>
                              <tr>
                                    <td><asp:HyperLink NavigateUrl="Account.aspx" Text="Go Back to Accounts Page" runat="server" Font-Size="20px" ValidationGroup="recPwdGroup1"></asp:HyperLink></td>
                                    <td><asp:Button id="recoverPwd" Text="Recover Password" runat="server" Width="250px" Height="35px" Font-Size ="25px" ForeColor="Brown" Font-Bold="true" OnClick="recover_password" ValidationGroup="recPwdGroup"></asp:Button></td>
                              </tr>
                               <tr><td>&nbsp</td><td>&nbsp</td></tr>
                          </table>
                       </fieldset>
                       <br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br />
                   </asp:Panel>

                   <!-- New Account -->
                    <asp:Panel id = "newAcount" runat="server">
                        <fieldset>
                           <legend class ="accountLegend">Create New Account</legend>
                           <br />
                           <table>
                               <tr>
                                   <td  colspan="2"><asp:Label id="newL1" Text = "Apply Online to open a New Account with us." runat="server" Font-Size="22px" ForeColor="Red"></asp:Label></td>
                               </tr>
                               <tr>
                                   <td  colspan="2"><asp:Label id="newL2" Text = "To complete an online application, please fill out the following information and then hit the 'Create Account' button." runat="server" ></asp:Label></td>
                               </tr>
                               <tr><td>&nbsp</td><td>&nbsp</td></tr>
                                <tr>
                                    <td><asp:Label id="newL3" Text = "Full Name" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Brown" ></asp:Label>
                                        <asp:Label id="astrik1" Text = "*" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Red"></asp:Label></td>

                                    <td><asp:TextBox id="newText1" runat="server" Width="75%" Height="25px" Font-Size ="20px" ></asp:TextBox>
                                   <asp:RequiredFieldValidator id="newTextReq1" runat="server" 
                                                            ErrorMessage="Name is Mandatory." 
                                                            ControlToValidate="newText1"  
                                                            ForeColor="Red"
                                                            ValidationGroup="newAccGroup"
                                                            Display ="Dynamic"/>

                                   </td>
                               </tr>
                                <tr>
                                    <td><asp:Label id="newL4" Text = "Company" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Brown"  ></asp:Label></td>
                                    <td><asp:TextBox id="newText2" runat="server" Width="75%" Height="25px" Font-Size ="20px"></asp:TextBox></td>
                                </tr>
                                <tr>
                                    <td><asp:Label id="newL5" Text = "Mailing Address" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Brown" ></asp:Label>
                                        <asp:Label id="astrik2" Text = "*" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Red"></asp:Label>
                                    </td>
                                    <td><asp:TextBox id="newText3" runat="server" Width="75%" Height="25px" Font-Size ="20px"></asp:TextBox>
                                        <asp:RequiredFieldValidator id="newTextReq3" runat="server" 
                                                            ErrorMessage="Address is Mandatory." 
                                                            ControlToValidate="newText3" 
                                                            ForeColor="Red"
                                                            ValidationGroup="newAccGroup"
                                                            Display ="Dynamic"/>

                                   </td>
                               </tr>
                                <tr>
                                    <td><asp:Label id="newL6" Text = "Phone Number" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Brown"  ></asp:Label>
                                        <asp:Label id="astrik3" Text = "*" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Red"></asp:Label>
                                    </td>
                                    <td><asp:TextBox id="newText4" runat="server" Width="75%" Height="25px" Font-Size ="20px"></asp:TextBox>
                                        <asp:RequiredFieldValidator id="newTextReq4" runat="server" 
                                                            ErrorMessage="Phone Number is Mandatory." 
                                                            ControlToValidate="newText4" 
                                                            ForeColor="Red"
                                                            ValidationGroup="newAccGroup"
                                                            Display ="Dynamic"/>
                                       <asp:RegularExpressionValidator ID="newTextReg4" runat="server" 
                                                ErrorMessage="Kindly Enter Valid Phone Number. Eg. XXX-XXX-XXXX" 
                                                ControlToValidate="newText4"     
                                                ValidationExpression="^[0-9]{3}-[0-9]{3}-[0-9]{4}$"
                                                ForeColor="Red"
                                                ValidationGroup="newAccGroup"
                                                Display ="Dynamic" />


                                   </td>
                               </tr>
                                <tr>
                                    <td><asp:Label id="newL7" Text = "Email Address" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Brown"></asp:Label>
                                        <asp:Label id="astrik4" Text = "*" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Red"></asp:Label>
                                    </td>
                                    <td><asp:TextBox id="newText5" runat="server" Width="75%" Height="25px" Font-Size ="20px">

                                        </asp:TextBox><asp:RequiredFieldValidator id="newTextReq5" runat="server" 
                                                            ErrorMessage="Email Id is Mandatory." 
                                                            ControlToValidate="newText5" 
                                                            ForeColor="Red"
                                                            ValidationGroup="newAccGroup"
                                                            Display ="Dynamic"/>
                                       <asp:RegularExpressionValidator ID="newTextReg5" runat="server"    
                                                ErrorMessage="Kindly Enter Valid Email-Id.  Eg. example@exmaple.com" 
                                                ControlToValidate="newText5"     
                                                ValidationExpression="^([\w\.\-]+)@([\w\-]+)((\.(\w){2,3})+)$"
                                                ForeColor="Red" 
                                                ValidationGroup="newAccGroup"
                                                Display ="Dynamic"/>

                                   </td>
                               </tr>
                                <tr>
                                    <td><asp:Label id="newL8" Text = "Access Code" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Brown"></asp:Label></td>
                                    <td><asp:TextBox id="newText6" BackColor="Black" ForeColor="WhiteSmoke" ReadOnly="true" runat="server" OnLoad ="getAccessCode" Width="75%" Height="25px" Font-Size ="20px"  Font-Italic ="True"></asp:TextBox></td>
                                </tr>
                                <tr>
                                    <td><asp:Label id="newL9" Text = "Enter Access Code" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Brown"  ></asp:Label>
                                        <asp:Label id="astrik5" Text = "*" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Red"></asp:Label>
                                    </td>
                                    <td><asp:TextBox id="newText7" runat="server" Width="75%" Height="25px" Font-Size ="20px" ></asp:TextBox>
                                        <asp:RequiredFieldValidator id="newTextReq7" runat="server" 
                                                            ErrorMessage="Access Code is Mandatory." 
                                                            ControlToValidate="newText7" 
                                                            ForeColor="Red"
                                                            ValidationGroup="newAccGroup"
                                                            Display ="Dynamic"/>
                                       <asp:CompareValidator id="newTextComp7" runat="server" 
                                                            ErrorMessage="Your Access Code does not match."
                                                            ControlToCompare="newText6" 
                                                            ControlToValidate="newText7" 
                                                            ForeColor="Red"
                                                            ValidationGroup="newAccGroup"
                                                            Display ="Dynamic"/>
                                   </td>
                               </tr>
                                <tr>
                                    <td>&nbsp</td>
                                    <td><asp:Button id="createAcc" Text="Create Account" runat="server" OnClick="createAccount" Width="200px" Height="35px" Font-Size ="25px" ForeColor="Brown" Font-Bold="true" ValidationGroup="newAccGroup"></asp:Button>
                                    <asp:Button id="noThanks" Text="Back To Accounts" runat="server" OnClick="noThanks_Click" Width="250px" Height="35px" Font-Size ="25px" ForeColor="Brown" Font-Bold="true" ValidationGroup="newAccGroup1"></asp:Button></td>
                                </tr>
                               <tr><td>&nbsp</td><td>&nbsp</td></tr>
                            </table>
                       </fieldset>
                       <br /><br /><br /><br /><br /><br /><br /><br /><br /><br />
                    </asp:Panel>


                   <!-- After Login -->
                   <asp:Panel id = "afterLogin" runat = "server">
                       <table>
                            <tr>
                                <td><asp:Label id="loginL1" runat="server" ForeColor="Purple" Font-Bold="true" Font-Size="30px"></asp:Label></td>
                                <td><asp:linkButton Text="Log Out" runat="server" Font-Size="20px" OnClick="logout_Click" ValidationGroup ="logoutGroup"></asp:linkButton></td>
                            </tr>
                           <tr><td>&nbsp</td><td>&nbsp</td></tr>
                       </table>
                       
                        <br />
                       <fieldset>
                           <legend class ="accountLegend">New Delivery Request</legend>
                           <br />
                           <table>
                                <tr>  
                                    <td colspan="2"><asp:Label id="loginL2" Text = "Please enter Delivery Details below" runat="server" Font-Size="22px" ForeColor="Red" ></asp:Label></td>
                                </tr>
                                <tr>
                                    <td><asp:Label id="log3nL2" Text = "Pickup Address" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Brown" ></asp:Label>
                                        <asp:Label id="astrik6" Text = "*" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Red"></asp:Label>
                                    </td>
                                    <td><asp:TextBox id="loginT1" runat="server"  Width="75%" Height="25px" Font-Size ="20px"></asp:TextBox>
                                        <asp:RequiredFieldValidator id="loginTReq1" runat="server" 
                                                            ErrorMessage="Address is Mandatory."
                                                            ValidationGroup="delGroup" 
                                                            ControlToValidate="loginT1" 
                                                            ForeColor="Red"
                                                            Display ="Dynamic"/>

                                   </td>
                               </tr>
                                <tr>
                                    <td><asp:Label id="loginL4" Text = "Recipient Address" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Brown" ></asp:Label>
                                        <asp:Label id="astrik7" Text = "*" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Red"></asp:Label></td>
                                    <td><asp:TextBox id="loginT2" runat="server"  Width="75%" Height="25px" Font-Size ="20px"></asp:TextBox>
                                        <asp:RequiredFieldValidator id="loginTReq2" runat="server" 
                                                            ErrorMessage="Recipient Address is Mandatory." 
                                                            ControlToValidate="loginT2" 
                                                            ForeColor="Red"
                                                            ValidationGroup="delGroup"
                                                            Display ="Dynamic"/>

                                   </td>
                               </tr>
                                <tr>
                                    <td><asp:Label id="loginL5" Text = "Recipient Phone" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Brown" ></asp:Label>
                                        <asp:Label id="astrik8" Text = "*" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Red"></asp:Label></td>
                                    <td><asp:TextBox id="loginT3" runat="server"  Width="75%" Height="25px" Font-Size ="20px"></asp:TextBox>
                                        <asp:RequiredFieldValidator id="loginTReq3" runat="server" 
                                                            ErrorMessage="Recipient Phone is Mandatory." 
                                                            ControlToValidate="loginT3" 
                                                            ForeColor="Red"
                                                            ValidationGroup="delGroup"
                                                            Display ="Dynamic"/>
                                       <asp:RegularExpressionValidator ID="loginTReg3" runat="server"     
                                                ErrorMessage="Kindly Enter Valid Recipient Phone Number. Eg. XXX-XXX-XXXX" 
                                                ControlToValidate="loginT3"     
                                                ValidationExpression="^[0-9]{3}-[0-9]{3}-[0-9]{4}$"
                                                ForeColor="Red" 
                                                ValidationGroup="delGroup"
                                                Display ="Dynamic"/>

                                   </td>
                               </tr>
                                <tr>

                                    <td><asp:Label id="loginL6" Text = "Provide a Description" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Brown" ></asp:Label>
                                        <asp:Label id="loginL7" Text = "(List Items to deliver)" runat="server" Font-Size ="21px" Font-Bold =" true" ForeColor="Brown" ></asp:Label>
                                        <asp:Label id="astrik9" Text = "*" runat="server" Font-Size ="25px" Font-Bold =" true" ForeColor="Red"></asp:Label>

                                    </td>
                                    <td><asp:TextBox id="loginT4" TextMode ="MultiLine" runat="server"  Width="75%" Height="100px" Font-Size ="20px">

                                        </asp:TextBox><asp:RequiredFieldValidator id="loginTReq4" runat="server" 
                                                            ErrorMessage="Description is Mandatory." 
                                                            ControlToValidate="loginT4" 
                                                            ForeColor="Red"
                                                            ValidationGroup="delGroup"
                                                            Display ="Dynamic"/>

                                   </td>
                               </tr>
                               <tr>
                                   <td>&nbsp</td>
                                    <td><asp:Button id="submitReq" Text="Submit Request" runat="server"  OnClick="submitRequest" Width="200px" Height="35px" Font-Size ="22px" ForeColor="Brown" Font-Bold="true" ValidationGroup="delGroup"></asp:Button>
                                      
                                    </td>
                               </tr>
                           <tr><td>&nbsp</td><td>&nbsp</td></tr>
                       </table>

                       </fieldset>
                       <br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br />
                    </asp:Panel>
                   

                 </div>       
                </div>
            </div>
        </div>   
    </form>
</body>
</html>
