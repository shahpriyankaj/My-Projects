<%@ Application Language="C#" %>
<%@ Import Namespace="System.Data.SqlClient" %>
<%@ Import Namespace="System.Data" %> 

<script runat="server">

    void Application_Start(object sender, EventArgs e) 
    {
        // Code that runs on application startup
        
        Application["AllUsersList"] = new List<Users>();
        Application["AllUsersList"] = null;
        List<Users> allUsersList = new List<Users>();

        String connectionString = ConfigurationManager.ConnectionStrings["connString"].ConnectionString;
        using (SqlConnection conn = new SqlConnection(connectionString))
        {
            string sql = "SELECT * from ShahP_WADsp16_Users";
            using (SqlCommand cmd = new SqlCommand(sql, conn))
            {
                try
                {
                    conn.Open();
                    SqlDataReader sqlReader = cmd.ExecuteReader();
                    
                    while (sqlReader.Read())
                    {
                        Users oldUser = new Users((String)sqlReader["fullName"], (String)sqlReader["company"], (String)sqlReader["mailingAddress"], (String)sqlReader["phoneNumber"], (String)sqlReader["emailAddress"], (int)sqlReader["accessCode"]);
                        using (SqlConnection conn1 = new SqlConnection(connectionString))
                        {
                            string sql1 = "SELECT * from ShahP_WADsp16_delivery_request1 where accessCode = " + (int)sqlReader["accessCode"];
                            using (SqlCommand cmd1 = new SqlCommand(sql1, conn1))
                            {
                                try
                                {
                                    conn1.Open();
                                    SqlDataReader sqlReader1 = cmd1.ExecuteReader();
                                    while (sqlReader1.Read())
                                    {
                                        DeliveryDetails oldDel = new DeliveryDetails((DateTime)sqlReader1["requestDate"], (String)sqlReader1["pickupAddress"], (String)sqlReader1["receipientAddress"], (String)sqlReader1["receipientPhone"], (String)sqlReader1["description"]);
                                        oldUser.DeliveryDetailsList.Add(oldDel);
                                    }

                                }
                                finally
                                {
                                    conn1.Close();
                                }
                                allUsersList.Add(oldUser);


                            }
                        }
                    }
                }
                finally
                {
                    conn.Close();
                    Application["AllUsersList"] = allUsersList;
                }
            }
        }

        
    }
    
    void Application_End(object sender, EventArgs e) 
    {
        //  Code that runs on application shutdown

    }
        
    void Application_Error(object sender, EventArgs e) 
    { 
        // Code that runs when an unhandled error occurs

    }

    void Session_Start(object sender, EventArgs e) 
    {
        // Code that runs when a new session is started

    }

    void Session_End(object sender, EventArgs e) 
    {
        // Code that runs when a session ends. 
        // Note: The Session_End event is raised only when the sessionstate mode
        // is set to InProc in the Web.config file. If session mode is set to StateServer 
        // or SQLServer, the event is not raised.

    }
       
</script>
