db.createUser({
    user : "dbuser",
    pwd : "dbpasswd",
    roles : [
        { role: "readWrite", 
          db:   "luca_db" 
        }
        
    ]


});