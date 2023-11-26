/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 16:28:11 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/26 16:39:27 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request( void ) {

}

Request::Request( Request const &rhs ) {
	*this = rhs;
}

Request &Request::operator=( Request const &rhs ) {
	if ( this != &rhs ) {
	}
	return *this;
}

Request::~Request( void ) {
}

// Getters