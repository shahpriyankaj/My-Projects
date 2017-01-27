using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Net;
using System.Net.Mail;
using System.Data;
using System.Data.SqlClient;
using System.Configuration;

public partial class _Default : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        if (Session["CurrentUser"] == null)
        {  
            existAcount.Visible = true;
            newAcount.Visible = false;
            afterLogin.Visible = false;
            radio123.Visible = true;
            recoverPassword.Visible = false;
        }
        else
        {
                existAcount.Visible = true;
                existAcount.Visible = false;
                newAcount.Visible = false;
                afterLogin.Visible = true;
                radio123.Visible = false;
                recoverPassword.Visible = false;
                Users user = Session["CurrentUser"] as Users;
                loginL1.Text = "Welcome " + user.FullName + "!";
        }
    }
    protected void accountIndexChanged(object sender, EventArgs e)
    {
        afterLogin.Visible = false;
        recoverPassword.Visible = false;
        
        if (string.Equals(accountRadio.SelectedValue, "existRadio"))
        {
            existAcount.Visible = true;
            newAcount.Visible = false;   
        }
        else
        {
            existAcount.Visible = false;
            newAcount.Visible = true;
        }

    }

    public bool send_email(String to, String header, String subject, String body)
    {
       // Logic for sending an email.
        MailMessage mail = new MailMessage();
        mail.To.Add(to);
        mail.From = new MailAddress("shahpriyankaj@gmail.com", header);
        mail.Subject = subject;
        mail.Body = body;
        mail.IsBodyHtml = true;
        SmtpClient client = new SmtpClient();
        client.Credentials = new NetworkCredential("shahpriyankaj@gmail.com", "prinishashah1990");
        client.Port = 587;
        client.Host = "smtp.gmail.com";
        client.EnableSsl = true;
        try
        {
            client.Send(mail);                   
        }
        catch (Exception ex)
        {
            Exception ex2 = ex;
            string errorMessage = string.Empty;
            while (ex2 != null)
            {
                errorMessage += ex2.ToString();
                ex2 = ex2.InnerException;
            }
            return false;
        }
        return true;
    }

    protected void logInAccount(object sender, EventArgs e)
    {
        if (!ValidateLogIn())
        {
            return;
        }
        
        String email = "";
        int accessCode = 0;
        bool userExist = false;

        if(Application["AllUsersList"] != null)
        {
            List<Users> allUsersList = Application["AllUsersList"] as List<Users>;

            email = emailText.Text;
            accessCode = Int32.Parse(pwdText.Text);

            for (int i = 0; i < allUsersList.Count; i++)
            {
                if (email.Equals(allUsersList[i].EmailAddress) && accessCode == allUsersList[i].AccessCode)
                {
                    ScriptManager.RegisterClientScriptBlock(this, this.GetType(), "", "alert('You have successfully logged in!')", true);
                    Session["CurrentUser"] = allUsersList[i];
                    existAcount.Visible = false;
                    newAcount.Visible = false;
                    afterLogin.Visible = true;
                    radio123.Visible = false;
                    recoverPassword.Visible = false;
                    loginL1.Text = "Welcome " + allUsersList[i].FullName + "!";
                    userExist = true;
                    break;
                }
            }
            if (userExist == false)
            {
                ScriptManager.RegisterClientScriptBlock(this, this.GetType(), "", "alert('Please enter correct Email Address or Access Code')", true);
            }
        }   
    }

    private bool ValidateLogIn()
    {
        validEmailReq.Validate();
        validEmailReg.Validate();
        validPwdReq.Validate();
        validPwdReg.Validate();
        if (!validEmailReq.IsValid || 
            !validEmailReg.IsValid || 
            !validPwdReq.IsValid || 
            !validPwdReg.IsValid)
        {
            return false;
        }
        return true;
    }

    protected void submitRequest(object sender, EventArgs e)
    {
        if (!ValidateSubmitReq())
        {
            return;
        }

        Users user = null;
       
        if (Session["CurrentUser"] != null)
        {
            user = Session["CurrentUser"] as Users;
        }

        var currDate = DateTime.Now;
        DeliveryDetails d = new DeliveryDetails(currDate, loginT1.Text, loginT2.Text, loginT3.Text, loginT4.Text);
  
        user.DeliveryDetailsList.Add(d); // Adding Delivery Details in an array

        // Storing into table Delivery Request.
        String connectionString = ConfigurationManager.ConnectionStrings["connString"].ConnectionString;
        SqlConnection conn = new SqlConnection(connectionString);

            String insToDelv = "Insert into ShahP_WADsp16_delivery_request1(accessCode, pickupAddress, receipientAddress, receipientPhone, description) " +
                                    "values(" + user.AccessCode + ", '" +
                                                loginT1.Text + "', '" +
                                                loginT2.Text + "', '" +
                                                loginT3.Text + "', '" +
                                                loginT4.Text + "')";
        SqlCommand cmd = new SqlCommand(insToDelv, conn);

        try
        {
            conn.Open();
            cmd.BeginExecuteNonQuery();

        }
        finally
        {
            conn.Close();
        }
            
        String header = "Delivery Request";
        String subject = "Texas Delivery Service - New Request Confirmation";
        String body = "Valued Customer " + user.FullName + ",<br/>You have requested a new delivery. Details shown below. <br/> <br/>" +
                      "Pickup Address : " + d.PickupAddress + "<br/>" +
                      "Recipient Address : " + d.ReceipientAddress + "<br/>" +
                      "Description : " + d.Description + "<br/><br/>" +
                      "Please note that any modifications made after 2 hours of submission of the original request shall be rejected.<br/><br/>" +
                      "Thank You again for keeping trust on us.<br/><br/>Texas Deivery Service Team";

        if (send_email(user.EmailAddress, header, subject, body))
        {
            int count = user.DeliveryDetailsList.Count;

            ScriptManager.RegisterClientScriptBlock(this, this.GetType(), "", "alert('Thank You for submitting a delivery request. We are more than happy to serve you. An invoice email has been sent to the email address in our file.')", true);
        }
        else
        {
            ScriptManager.RegisterClientScriptBlock(this, this.GetType(), "", "alert('We could not process your request at this time.')", true);
        }
        existAcount.Visible = false;
        newAcount.Visible = false;
        afterLogin.Visible = true;
        radio123.Visible = false;
        recoverPassword.Visible = false;
    }

    private bool ValidateSubmitReq()
    {
        if (!loginTReq1.IsValid)
        {
            loginTReq1.Validate();
        }

        if (!loginTReq2.IsValid)
        {
            loginTReq2.Validate();
        }

        if (!loginTReq3.IsValid)
        {
            loginTReq3.Validate();
        }

        if (!loginTReg3.IsValid)
        {
            loginTReg3.Validate();
        }

        if (!loginTReq4.IsValid)
        {
            loginTReq4.Validate();
        }
        

        if (!loginTReq1.IsValid || 
            !loginTReq2.IsValid || 
            !loginTReq3.IsValid || 
            !loginTReg3.IsValid || 
            !loginTReq4.IsValid)
        {
            return false;
        }

        return true;
    }

    protected void createAccount(object sender, EventArgs e)
    {
        if (!ValidateCreateAcc())
        {
            return;
        }
        String email = "";
        int accessCode = 0;
        bool userExist = false;
        if (Application["AllUsersList"] != null)
        {
            List<Users> allUsersList = Application["AllUsersList"] as List<Users>;
            
            email = newText5.Text;
            accessCode = Int32.Parse(newText7.Text);

            for (int i = 0; i < allUsersList.Count; i++)
            {
                if (Application["AllUsersList"] != null && allUsersList[i].EmailAddress.Equals(email)) //&& accessCode == allUsersList[i].AccessCode)
                {
                    ScriptManager.RegisterClientScriptBlock(this, this.GetType(), "", "alert('Your email address is already registered in our database. Please login!')", true);
                    existAcount.Visible = false;
                    newAcount.Visible = true;
                    afterLogin.Visible = false;
                    radio123.Visible = true;
                    recoverPassword.Visible = false;
                    userExist = true;
                    break;
                }
            }
        }
        if (userExist == false)
        {
            Users newUser = new Users(newText1.Text, newText2.Text, newText3.Text, newText4.Text, newText5.Text, Int32.Parse(newText7.Text));
            List<Users> allUsersList = Application["AllUsersList"] as List<Users>;
            allUsersList.Add(newUser);
            Application["AllUsersList"] = allUsersList;

            // Storing into table Users.
            String connectionString = ConfigurationManager.ConnectionStrings["connString"].ConnectionString;
            SqlConnection conn = new SqlConnection(connectionString);
            
            String insToUSer = "Insert into ShahP_WADsp16_Users(accessCode, fullName, company, mailingAddress, phoneNumber, emailAddress) " +
                                    "values(" + Int32.Parse(newText7.Text) + ", '" +
                                                newText1.Text + "', '" +
                                                newText2.Text + "', '" +
                                                newText3.Text + "', '" +
                                                newText4.Text + "', '" +
                                                newText5.Text + "')";
            SqlCommand cmd = new SqlCommand(insToUSer, conn);

            try
            {
                conn.Open();
                cmd.BeginExecuteNonQuery();

            }
            catch
            {
                ScriptManager.RegisterClientScriptBlock(this, this.GetType(), "", "alert('catch')", true);
            }
            finally
            {
                conn.Close();
            }
                
            String header = "Registeration";
            String subject = "Texas Delivery Service - Registration Confirmation";
            String body = "Valued Customer " + newUser.FullName + ",<br/>Thank you for registering with Texas Delivery Service Name. You can now start using your account to " +
                                                                "request us to deliver your goods.<br/><br/>" +
                                                                "Looking forward to business with you.<br/>" +
                                                                "Thank You again for keeping trust on us.<br/><br/>Texas Deivery Service – Security Team";
            if (send_email(email, header, subject, body))
            {
                ScriptManager.RegisterClientScriptBlock(this, this.GetType(), "", "alert('Thank You for registering. You can now login.')", true);
                existAcount.Visible = true;
                newAcount.Visible = false;
                afterLogin.Visible = false;
                radio123.Visible = true;
                accountRadio.SelectedValue = "existRadio";
                recoverPassword.Visible = false;
            }
            else
            {
                ScriptManager.RegisterClientScriptBlock(this, this.GetType(), "", "alert('We could not register at this time.')", true);
            }
        }
  
    }

    private bool ValidateCreateAcc()
    {
        newTextReq1.Validate();
        newTextReq3.Validate();
        newTextReq4.Validate();
        newTextReg4.Validate();
        newTextReq5.Validate();
        newTextReg5.Validate();
        newTextReq7.Validate();
       if (!newTextComp7.IsValid)
       { 
            newTextComp7.Validate();
        }

        if (!newTextReq1.IsValid || 
            !newTextReq3.IsValid || 
            !newTextReq4.IsValid || 
            !newTextReg4.IsValid || 
            !newTextReq5.IsValid ||
            !newTextReg5.IsValid ||
            !newTextReq7.IsValid ||
            !newTextComp7.IsValid)
        {
            return false;
        }

        return true;
    }

    protected void getAccessCode(object sender, EventArgs e)
    {
        Random generator = new Random();
        String r = generator.Next(0, 1000000).ToString("D6");
        newText6.Text = r;
    }

    protected void noThanks_Click(object sender, EventArgs e)
    {
        Response.Redirect("Account.aspx");
    }
    protected void cancelReq_Click(object sender, EventArgs e)
    {
        existAcount.Visible = false;
        newAcount.Visible = false;
        afterLogin.Visible = true;
        radio123.Visible = false;
        recoverPassword.Visible = false;
    }

    protected void recover_password(object sender, EventArgs e)
    {
        if(!ValidateForgetCode())
        {
            return;
        }
        String email = "";
        bool userExist = false;
        
         
        if (Application["AllUsersList"] != null)
        {
            List<Users> allUsersList = Application["AllUsersList"] as List<Users>;

            email = emailText1.Text;
            int rAccessCode = 0;
            for (int i = 0; i < allUsersList.Count; i++)
            {
                if (email.Equals(allUsersList[i].EmailAddress))
                {
                    
                        // get accessCode from the database
                        String connectionString = ConfigurationManager.ConnectionStrings["connString"].ConnectionString;
                        SqlConnection conn = new SqlConnection(connectionString);

                        String selToUSer = "select accessCode from ShahP_WADsp16_Users" + 
                                                            " where emailAddress = '" +
                                                             allUsersList[i].EmailAddress + "'";
                        SqlCommand cmd = new SqlCommand(selToUSer, conn);

                        try
                        {
                            conn.Open();
                            SqlDataReader sqlReader = cmd.ExecuteReader();
                            while (sqlReader.Read())
                            {
                                rAccessCode = (int)sqlReader["accessCode"];
                            }
                                String header = "Recover Password";
                                String subject = "We have recovered your Access Code!";
                                String body = "Valued Customer,<br/>Thank you for contacting us. Your access code is " + rAccessCode.ToString() + ".<br/>Thank You.<br/><br/>Texas Deivery Service – Security Team";
                                if (send_email(email, header, subject, body))
                                {
                                    ScriptManager.RegisterClientScriptBlock(this, this.GetType(), "", "alert('Your Access Code has been sent to the Email Address.')", true);
                                }
                            }

                        
                        finally
                        {
                            conn.Close();
                        }
                        existAcount.Visible = true;
                        newAcount.Visible = false;
                        afterLogin.Visible = false;
                        radio123.Visible = true;
                        accountRadio.SelectedValue = "existRadio";
                        recoverPassword.Visible = false;
                        userExist = true;
                        break;
                    }
                    
                    
                }
            }
            
        
        if (ValidateForgetCode() && userExist == false)
        {
            ScriptManager.RegisterClientScriptBlock(this, this.GetType(), "", "alert('Your Account Email cannot be verified.')", true);
            existAcount.Visible = false;
            newAcount.Visible = false;
            afterLogin.Visible = false;
            radio123.Visible = false;
            recoverPassword.Visible = true;
        }
    }

    private bool ValidateForgetCode()
    {
        validEmailReq1.Validate();
        validEmailReg1.Validate();

        if (!validEmailReq1.IsValid ||
            !validEmailReg1.IsValid)
        {
            return false;
        }
        return true;
    }
    
    protected void get_access_code_Click(object sender, EventArgs e)
    {
        existAcount.Visible = false;
        newAcount.Visible = false;
        afterLogin.Visible = false;
        radio123.Visible = false;
        recoverPassword.Visible = true;
    }

    protected void logout_Click(object sender, EventArgs e)
    {
        Users user = null;

        if (Session["CurrentUser"] != null)
        {
            user = Session["CurrentUser"] as Users;
        }
        if (Application["AllUsersList"] != null)
        {
            List<Users> allUsersList = Application["AllUsersList"] as List<Users>;


            for (int i = 0; i < allUsersList.Count; i++)
            {
                if (user.EmailAddress.Equals(allUsersList[i].EmailAddress))
                {
                    allUsersList[i] = user;
                    break;
                }
            }

            Application["AllUsersList"] = allUsersList;
            Session.Abandon();
        }
        existAcount.Visible = true;
        newAcount.Visible = false;
        afterLogin.Visible = false;
        radio123.Visible = true;
        recoverPassword.Visible = false;
    }

    private void resetAllFields()
    {
        emailText.Text = "";
        pwdText.Text = "";
        emailText1.Text = "";
        newText1.Text = "";
        newText2.Text = "";
        newText3.Text = "";
        newText4.Text = "";
        newText5.Text = "";
        newText7.Text = "";
        loginT1.Text = "";
        loginT2.Text = "";
        loginT3.Text = "";
        loginT4.Text = "";
    }
}