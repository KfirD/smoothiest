//
//  MainViewController.swift
//  Smoothiest
//
//  Created by Keegan Papakipos on 7/29/17.
//  Copyright © 2017 kpapakipos. All rights reserved.
//

import UIKit

class MainViewController: UIViewController, UITableViewDataSource, UITableViewDelegate {

    @IBOutlet weak var amountField: UITextField!
    @IBOutlet weak var chosenIngredientsView: UITextView!
    @IBOutlet weak var ingredientsTableView: UITableView!
    @IBOutlet weak var ingredientsSearchBar: UISearchBar!
    
    let possibleIngredients = [Ingredient("Strawberries", image: UIImage()),Ingredient("Mangoes", image: UIImage())]
    var chosenIngredients = [Ingredient]()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        ingredientsTableView.dataSource = self
        ingredientsTableView.delegate = self
    }
    
    @IBAction func deletePressed(_ sender: Any) {
        repeat {
            if chosenIngredientsView.text.characters.count > 0 {
                chosenIngredientsView.deleteBackward()
            } else {
                break
            }
        } while chosenIngredientsView.text.characters.last != ","
    }

    @IBAction func gimmeASmoothiePressed(_ sender: Any) {
        var ingredients = [Ingredient]()
        let smoothieAmount = Float(amountField.text!)
        for chosenIngredient in chosenIngredients {
            ingredients.append(chosenIngredient)
        }
        
        guard userErrorCheck(ingredients: ingredients, smoothieAmount: smoothieAmount) == true else {
            return
        }
        let ingredientAmountsResult = NetworkController.getIngredientAmountsFromServer(ingredients: ingredients, smoothieAmountInOz: smoothieAmount!)
        print(ingredientAmountsResult)
        performSegue(withIdentifier: "displaySmoothieResultsSegue", sender: nil)
    }
    
    func userErrorCheck(ingredients: [Ingredient], smoothieAmount: Float?) -> Bool {
        guard ingredients.count > 0 else {
            print("No ingredients selected")
            return false
        }
        guard smoothieAmount != nil else {
            print("Non-number detected in smoothie amount field")
            return false
        }
        let unwrappedSmoothieAmount = smoothieAmount!
        guard unwrappedSmoothieAmount > 0 else {
            print("Smoothie amount field must be greater than zero")
            return false
        }
        return true
    }
    
    // MARK: - Table view data source
    
    func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return possibleIngredients.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "ingredientCell", for: indexPath) as!IngredientTableViewCell
        
        cell.ingredient = possibleIngredients[indexPath.row]
        cell.nameLabel.text = possibleIngredients[indexPath.row].name
        cell.imageView?.image = possibleIngredients[indexPath.row].image
        
        return cell
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        if (chosenIngredientsView.text?.characters.count)! > 0 {
            chosenIngredientsView.text?.append(", ")
        }
        let cell = ingredientsTableView.cellForRow(at: indexPath) as! IngredientTableViewCell
        cell.setSelected(false, animated: true)
        if let ing = cell.ingredient {
            chosenIngredients.append(ing)
            chosenIngredientsView.text?.append(ing.name)
        } else {
            print("Cell does not contain an ingredient")
            return
        }
    }
    
//     override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
//        
//     }
}
