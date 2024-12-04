import express from "express";
import bcrypt from "bcrypt";
import jwt from "jsonwebtoken";
import dotenv from "dotenv";

dotenv.config();
const app = express()
app.use(express.json());

const posts = [
    {
        username: "Sky",
        title: "dam"
    }
]

const users = [
    {
        username: "Sky",
        password: "dam"
    }
]

function authenticate(req, res, next){
    const authHeader = req.headers["authorization"];
    const token = authHeader && authHeader.split(" ")[1];
    if(!token)
        return res.status(401).send();

    jwt.verify(token, process.env.ACCESSTOKENSCRT, (err, user) => {
        if(err) return res.status(403).send();
        req.user = user;
        next()
    }) ;
}

app.get("/hi", (req, res) => {
    return res.json(posts);
});

app.get("/users", authenticate, (req, res) => {
    return res.json(users.filter(user => user.name === req.user.name));
});

app.listen(4000)