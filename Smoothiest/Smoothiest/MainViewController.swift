//
//  MainViewController.swift
//  Smoothiest
//
//  Created by Keegan Papakipos on 7/29/17.
//  Copyright © 2017 kpapakipos. All rights reserved.
//

import UIKit

class MainViewController: UIViewController, UITableViewDataSource, UITableViewDelegate, UISearchBarDelegate {

    @IBOutlet weak var amountLabel: UILabel!
    @IBOutlet weak var chosenIngredientsView: UITextView!
    @IBOutlet weak var ingredientsTableView: UITableView!
    @IBOutlet weak var ingredientsSearchBar: UISearchBar!
    
    let possibleIngredients = [Ingredient("Strawberries", image: UIImage(named: "Strawberries.jpg")!),Ingredient("Mangoes", image: UIImage(named: "Mangoes.jpg")!), Ingredient("Bananas", image: UIImage(named: "Bananas.jpg")!), Ingredient("Pineapple", image: UIImage(named: "Pineapples.jpg")!)].sorted {$0.name < $1.name}
    var filteredPossibleIngredients = [Ingredient]()
    var chosenIngredients = [Ingredient]()
    var smoothieAmount = Float()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        ingredientsTableView.dataSource = self
        ingredientsTableView.delegate = self
        ingredientsSearchBar.delegate = self
        filteredPossibleIngredients = possibleIngredients
        amountLabel.text = "You're making: \(smoothieAmount) oz"
    }
    
    @IBAction func backPressed(_ sender: Any) {
        self.navigationController?.popToRootViewController(animated: true)
    }
    
    @IBAction func deletePressed(_ sender: Any) {
        if chosenIngredients.count > 0 {
            chosenIngredients.removeLast()
        }
        repeat {
            if chosenIngredientsView.text.characters.count > 0 {
                chosenIngredientsView.deleteBackward()
            } else {
                break
            }
        } while chosenIngredientsView.text.characters.last != ","
        if chosenIngredientsView.text.characters.count > 0 {
            chosenIngredientsView.deleteBackward()
        } else {
            return
        }
    }

    @IBAction func gimmeASmoothiePressed(_ sender: Any) {
        var ingredients = [Ingredient]()
        for chosenIngredient in chosenIngredients {
            ingredients.append(chosenIngredient)
        }
        
        guard userErrorCheck(ingredients: ingredients, smoothieAmount: smoothieAmount) == true else {
            return
        }
        NetworkController.getIngredientAmountsFromServer(ingredients: ingredients, smoothieAmountInOz: smoothieAmount, completion: smoothieResultsCallback)
    }
    
    func smoothieResultsCallback(ingredientAmountResults: [String:Any]) {
        print(ingredientAmountResults)
        if let status = ingredientAmountResults["status"] as? String {
            guard status == "200" else {
                print("JSON status was not 200.")
                return
            }
        } else {
            print("JSON did not contain a string status")
            return
        }
        guard let results = ingredientAmountResults["results"] as? [String:String] else {
            print("JSON did not contain a [String:String] results")
            return
        }
        print(results)
        DispatchQueue.main.async {
            self.performSegue(withIdentifier: "displaySmoothieResultsSegue", sender: results)
        }
    }
    
    func userErrorCheck(ingredients: [Ingredient], smoothieAmount: Float?) -> Bool {
        guard ingredients.count > 0 else {
            self.alert(message: "No ingredients selected")
            return false
        }
        return true
    }
    
    // MARK: - Table view data source
    
    func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return filteredPossibleIngredients.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "ingredientCell", for: indexPath) as!IngredientTableViewCell
        
        cell.ingredient = filteredPossibleIngredients[indexPath.row]
        cell.nameLabel.text = cell.ingredient?.name
        cell.imageView?.image = cell.ingredient?.image
        cell.setNeedsLayout()
        cell.layoutIfNeeded()
        
        return cell
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let cell = ingredientsTableView.cellForRow(at: indexPath) as! IngredientTableViewCell
        cell.setSelected(false, animated: true)
        guard let ing = cell.ingredient else {
            self.alert(message: "selected cell does not contain ingredient")
            return
        }
        guard !chosenIngredientsView.text.contains(ing.name) else {
            return
        }
        if (chosenIngredientsView.text?.characters.count)! > 0 {
            chosenIngredientsView.text?.append(", ")
        }
        chosenIngredients.append(ing)
        chosenIngredientsView.text?.append(ing.name)
    }
    
    // MARK: Searching Stuff
    
    func searchBar(_ searchBar: UISearchBar, textDidChange searchText: String) {
        if searchText == "" {
            filteredPossibleIngredients = possibleIngredients
        }
        else {
            filteredPossibleIngredients = possibleIngredients.filter {$0.name.lowercased().contains(searchText.lowercased())}
        }
        ingredientsTableView.reloadData()
    }
    
     override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        guard let results = sender as? [String:String] else {
            print("sender in prepareforsegue was not a [String:String]")
            return
        }
        (segue.destination as! SmoothieAmountsViewController).results = results
        (segue.destination as! SmoothieAmountsViewController).smoothieAmount = smoothieAmount
     }
}
